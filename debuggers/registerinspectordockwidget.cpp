#include "registerinspectordockwidget.h"
#include "ui_registerinspectordockwidget.h"

#include "emulator_core.h"

#include "dbg_cnesmappers.h"

#include "main.h"

RegisterInspectorDockWidget::RegisterInspectorDockWidget(regDBFunc regDB,QWidget *parent) :
    CDebuggerBase(parent),
    ui(new Ui::RegisterInspectorDockWidget)
{
   ui->setupUi(this);
   binaryModel = new CDebuggerRegisterDisplayModel(regDB);
   binaryDelegate = new CDebuggerNumericItemDelegate();
   ui->binaryView->setModel(binaryModel);
   ui->binaryView->setItemDelegate(binaryDelegate);
   bitfieldModel = new CDebuggerBitfieldDisplayModel(regDB);
   bitfieldDelegate = new CDebuggerBitfieldComboBoxDelegate();
   ui->bitfieldView->setModel(bitfieldModel);
   ui->bitfieldView->setItemDelegate(bitfieldDelegate);
   m_regDB = regDB;
   m_register = 0;

   ui->label->setText ( "" );

   // Connect signals to the UI to have the UI update.
   QObject::connect ( emulator, SIGNAL(cartridgeLoaded()), this, SLOT(updateMemory()) );
   QObject::connect ( emulator, SIGNAL(emulatorPaused(bool)), this, SLOT(updateMemory()) );
   QObject::connect ( breakpointWatcher, SIGNAL(breakpointHit()), this, SLOT(updateMemory()) );

   // Connect signals to the models to have the model update.
   QObject::connect ( emulator, SIGNAL(cartridgeLoaded()), binaryModel, SLOT(update()));
   QObject::connect ( emulator, SIGNAL(emulatorReset()), binaryModel, SLOT(update()) );
   QObject::connect ( emulator, SIGNAL(emulatorPaused(bool)), binaryModel, SLOT(update()) );
   QObject::connect ( breakpointWatcher, SIGNAL(breakpointHit()), binaryModel, SLOT(update()) );
   QObject::connect ( emulator, SIGNAL(cartridgeLoaded()), bitfieldModel, SLOT(update()));
   QObject::connect ( emulator, SIGNAL(emulatorReset()), bitfieldModel, SLOT(update()) );
   QObject::connect ( emulator, SIGNAL(emulatorPaused(bool)), bitfieldModel, SLOT(update()) );
   QObject::connect ( breakpointWatcher, SIGNAL(breakpointHit()), bitfieldModel, SLOT(update()) );

   // Connect inter-model signals so the models can update each other.
   QObject::connect ( bitfieldModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), binaryModel, SLOT(update()) );
   QObject::connect ( binaryModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), bitfieldModel, SLOT(update()) );
   QObject::connect ( ui->binaryView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(binaryView_currentChanged(QModelIndex,QModelIndex)) );
}

RegisterInspectorDockWidget::~RegisterInspectorDockWidget()
{
   delete ui;
   delete binaryModel;
   delete binaryDelegate;
   delete bitfieldModel;
   delete bitfieldDelegate;
}

void RegisterInspectorDockWidget::showEvent(QShowEvent* e)
{
   QObject::connect ( emulator, SIGNAL(updateDebuggers()), binaryModel, SLOT(update()));
   QObject::connect ( emulator, SIGNAL(updateDebuggers()), bitfieldModel, SLOT(update()));
   updateMemory();
   ui->binaryView->resizeColumnsToContents();
}

void RegisterInspectorDockWidget::hideEvent(QHideEvent* e)
{
   QObject::disconnect ( emulator, SIGNAL(updateDebuggers()), binaryModel, SLOT(update()));
   QObject::disconnect ( emulator, SIGNAL(updateDebuggers()), bitfieldModel, SLOT(update()));
}

void RegisterInspectorDockWidget::contextMenuEvent(QContextMenuEvent*)
{
}

void RegisterInspectorDockWidget::changeEvent(QEvent* e)
{
   CDebuggerBase::changeEvent(e);

   switch (e->type())
   {
      case QEvent::LanguageChange:
         ui->retranslateUi(this);
         break;
      default:
         break;
   }
}

void RegisterInspectorDockWidget::updateMemory ()
{
   char buffer [ 128 ];

   ui->binaryView->resizeColumnsToContents();

   if ( m_regDB() )
   {
      sprintf ( buffer, "%04X: %s", m_regDB()->GetRegister(m_register)->GetAddr(), m_regDB()->GetRegister(m_register)->GetName() );
      ui->label->setText ( buffer );
   }
#if 0
   CBreakpointInfo* pBreakpoints = nesGetBreakpointDatabase();
   eMemoryType memoryType = binaryModel->memoryType();
   int idx;
   int row = 0, col = 0;
   int low = 0, high = 0;
   int itemActual;
   char buffer [ 128 ];

   if ( m_regDB == nesGetCartridgeRegisterDatabase )
   {
      sprintf ( buffer, "Mapper %d: %s Register Inspector", nesGetMapper(), mapperNameFromID(nesGetMapper()) );
      setWindowTitle ( buffer );
   }

   // Check breakpoints for hits and highlight if necessary...
   for ( idx = 0; idx < pBreakpoints->GetNumBreakpoints(); idx++ )
   {
      BreakpointInfo* pBreakpoint = pBreakpoints->GetBreakpoint(idx);

      if ( pBreakpoint->hit )
      {
         if ( (pBreakpoint->type == eBreakOnOAMPortalAccess) ||
               (pBreakpoint->type == eBreakOnOAMPortalRead) ||
               (pBreakpoint->type == eBreakOnOAMPortalWrite) )
         {
            // Check memory range...
            low = binaryModel->memoryBottom();
            high = binaryModel->memoryTop();

            if ( (pBreakpoint->itemActual >= low) &&
                 (pBreakpoint->itemActual <= high) )
            {
               if ( ((pBreakpoint->target == eBreakInPPU) &&
                     (memoryType == eMemory_PPUoam)) )
               {
                  // Change memory address into row/column of display...
                  itemActual = pBreakpoint->itemActual - binaryModel->memoryBottom();
                  row = itemActual/binaryModel->columnCount();
                  col = itemActual%binaryModel->columnCount();

                  // Update display...
                  show();
                  ui->binaryView->resizeColumnsToContents();
                  ui->bitfieldView->resizeColumnsToContents();
                  ui->binaryView->setCurrentIndex(binaryModel->index(row,col));
                  on_binaryView_clicked(binaryModel->index(row,col));
                  ui->bitfieldView->setCurrentIndex(bitfieldModel->index(row,col));
               }
            }
         }
         else if ( (pBreakpoint->type == eBreakOnCPUState) ||
                   (pBreakpoint->type == eBreakOnPPUState) ||
                   (pBreakpoint->type == eBreakOnAPUState) ||
                   (pBreakpoint->type == eBreakOnMapperState) )
         {
            if ( ((pBreakpoint->target == eBreakInCPU) &&
                  (memoryType == eMemory_CPUregs)) ||
                  ((pBreakpoint->target == eBreakInPPU) &&
                   (memoryType == eMemory_PPUregs)) ||
                  ((pBreakpoint->target == eBreakInAPU) &&
                   (memoryType == eMemory_IOregs)) ||
                  ((pBreakpoint->target == eBreakInMapper) &&
                   (memoryType == eMemory_cartMapper)) )
            {
               // Change register into row/column of display...
               row = pBreakpoint->item1/binaryModel->columnCount();
               col = pBreakpoint->item1%binaryModel->columnCount();

               // Update display...
               show();
               ui->binaryView->resizeColumnsToContents();
               ui->bitfieldView->resizeColumnsToContents();
               ui->binaryView->setCurrentIndex(binaryModel->index(row,col));
               on_binaryView_clicked(binaryModel->index(row,col));
               ui->bitfieldView->setCurrentIndex(bitfieldModel->index(pBreakpoint->item2,0));
            }
         }
      }
   }
#endif
}

void RegisterInspectorDockWidget::binaryView_currentChanged(QModelIndex index, QModelIndex)
{
    char buffer [ 128 ];
    int cols = index.model()->columnCount();
    m_register = (index.row()*cols)+index.column();

    if ( m_regDB() )
    {
       sprintf ( buffer, "%04X: %s", m_regDB()->GetRegister(m_register)->GetAddr(), m_regDB()->GetRegister(m_register)->GetName() );
       ui->label->setText ( buffer );
    }

    bitfieldModel->setRegister ( m_register );
    bitfieldModel->update();
}

void RegisterInspectorDockWidget::on_binaryView_clicked(QModelIndex index)
{
   char buffer [ 128 ];
   int cols = index.model()->columnCount();
   m_register = (index.row()*cols)+index.column();

   if ( m_regDB() )
   {
      sprintf ( buffer, "%04X: %s", m_regDB()->GetRegister(m_register)->GetAddr(), m_regDB()->GetRegister(m_register)->GetName() );
      ui->label->setText ( buffer );
   }

   bitfieldModel->setRegister ( m_register );
   bitfieldModel->update();
}

void RegisterInspectorDockWidget::on_binaryView_doubleClicked(QModelIndex index)
{
   char buffer [ 128 ];
   int cols = index.model()->columnCount();
   m_register = (index.row()*cols)+index.column();

   if ( m_regDB() )
   {
      sprintf ( buffer, "%04X: %s", m_regDB()->GetRegister(m_register)->GetAddr(), m_regDB()->GetRegister(m_register)->GetName() );
      ui->label->setText ( buffer );
   }

   bitfieldModel->setRegister ( m_register );
   bitfieldModel->update();
}

void RegisterInspectorDockWidget::on_binaryView_pressed(QModelIndex index)
{
   char buffer [ 128 ];
   int cols = index.model()->columnCount();
   m_register = (index.row()*cols)+index.column();

   if ( m_regDB() )
   {
      sprintf ( buffer, "%04X: %s", m_regDB()->GetRegister(m_register)->GetAddr(), m_regDB()->GetRegister(m_register)->GetName() );
      ui->label->setText ( buffer );
   }

   bitfieldModel->setRegister ( m_register );
   bitfieldModel->update();
}

void RegisterInspectorDockWidget::on_binaryView_activated(QModelIndex index)
{
   char buffer [ 128 ];
   int cols = index.model()->columnCount();
   m_register = (index.row()*cols)+index.column();

   if ( m_regDB() )
   {
      sprintf ( buffer, "%04X: %s", m_regDB()->GetRegister(m_register)->GetAddr(), m_regDB()->GetRegister(m_register)->GetName() );
      ui->label->setText ( buffer );
   }

   bitfieldModel->setRegister ( m_register );
   bitfieldModel->update();
}

void RegisterInspectorDockWidget::on_binaryView_entered(QModelIndex index)
{
   char buffer [ 128 ];
   int cols = index.model()->columnCount();
   m_register = (index.row()*cols)+index.column();

   if ( m_regDB() )
   {
      sprintf ( buffer, "%04X: %s", m_regDB()->GetRegister(m_register)->GetAddr(), m_regDB()->GetRegister(m_register)->GetName() );
      ui->label->setText ( buffer );
   }

   bitfieldModel->setRegister ( m_register );
   bitfieldModel->update();
}

void RegisterInspectorDockWidget::on_bitfieldView_clicked(QModelIndex index)
{
   bitfieldDelegate->setBitfield ( m_regDB()->GetRegister(m_register)->GetBitfield(index.row()) );
   binaryModel->update();
}

void RegisterInspectorDockWidget::on_bitfieldView_doubleClicked(QModelIndex index)
{
   bitfieldDelegate->setBitfield ( m_regDB()->GetRegister(m_register)->GetBitfield(index.row()) );
   binaryModel->update();
}