#include "cdebuggerregistercomboboxdelegate.h"
#include "cnesicidecommon.h"

CDebuggerRegisterComboBoxDelegate::CDebuggerRegisterComboBoxDelegate()
   : m_pBitfield(NULL)
{}

QWidget *CDebuggerRegisterComboBoxDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
{
   int idx;

   if ( m_pBitfield )
   {
      if ( m_pBitfield->GetNumValues() )
      {
         QComboBox *editor = new QComboBox(parent);
         for ( idx = 0; idx < m_pBitfield->GetNumValues(); idx++ )
         {
            editor->addItem ( m_pBitfield->GetValueByIndex(idx), QVariant(idx) );
         }
         editor->setEditable(false);
         return editor;
      }
      else
      {
         QLineEdit *editor = new QLineEdit(parent);
         return editor;
      }
   }
   else
   {
      QLineEdit *editor = new QLineEdit(parent);
      return editor;
   }
}

void CDebuggerRegisterComboBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
   // get register value from model...
   bool ok;
   unsigned char value;

   // reduce to the bits we care about...
   if ( m_pBitfield )
   {
      value = m_pBitfield->GetValueRaw ( index.model()->data(index, Qt::EditRole).toString().toInt(&ok,16) );
      if ( m_pBitfield->GetNumValues() )
      {
         QComboBox *comboBox = static_cast<QComboBox*>(editor);
         comboBox->setCurrentIndex(value);
      }
      else
      {
         QLineEdit *edit = static_cast<QLineEdit*>(editor);
         edit->setText(index.model()->data(index, Qt::EditRole).toString());
      }
   }
   else
   {
      QLineEdit *edit = static_cast<QLineEdit*>(editor);
      edit->setText(index.model()->data(index, Qt::EditRole).toString());
   }
}

void CDebuggerRegisterComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
   // get register value from model...
   bool ok;
   unsigned char value;
   char data [ 8 ];

   if ( m_pBitfield )
   {
      if ( m_pBitfield->GetNumValues() )
      {
         QComboBox *comboBox = static_cast<QComboBox*>(editor);

         int bitfieldValue = comboBox->currentIndex();

         value = m_pBitfield->InsertValue ( index.model()->data(index, Qt::EditRole).toString().toInt(&ok,16), bitfieldValue );

         sprintf ( data, "%02X", value );

         model->setData(index, data, Qt::EditRole);
      }
      else
      {
         QLineEdit* edit = static_cast<QLineEdit*>(editor);

         value = m_pBitfield->InsertValue ( index.model()->data(index, Qt::EditRole).toString().toInt(&ok,16), edit->text().toInt(&ok,16) );

         sprintf ( data, "%02X", value );

         model->setData(index, data, Qt::EditRole);
      }
   }
}

void CDebuggerRegisterComboBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}