#ifndef CPROJECTMODEL_H
#define CPROJECTMODEL_H

#include "model/iuuidvisitor.h"
#include <QObject>

class CNesicideProject;

class CAttributeModel;
class CBinaryFileModel;
class CCartridgeModel;
class CFilterModel;
class CGraphicsBankModel;
class CSourceFileModel;
class CTileStampModel;

class CProjectModel : public QObject
{
   Q_OBJECT
signals:
   void itemAdded(QUuid item);
   void itemRemoved(QUuid item);

   void reset();

public:
   CProjectModel();
   ~CProjectModel();

   void setProject(CNesicideProject* project);

   // Retrieve a list of all Uuids present in the project.
   QList<QUuid> getUuids() const;

   // Accepts a IUuidVisitor that receives additional type information
   // for the given Uuid. This allows making choices on the type of
   // underlying data without exposing data outside of models.
   // Furthermore, this prevents chained calls to getModel->contains(uuid)
   // methods.
   void visitDataItem(const QUuid& uuid, IUuidVisitor& visitor);

   // Submodel retrieval.
   CAttributeModel*     getAttributeModel()     { return m_pAttributeModel; }
   CBinaryFileModel*    getBinaryFileModel()    { return m_pBinaryFileModel; }
   CCartridgeModel*     getCartridgeModel()     { return m_pCartridgeModel; }
   CFilterModel*        getFilterModel()        { return m_pFilterModel; }
   CGraphicsBankModel*  getGraphicsBankModel()  { return m_pGraphicsBankModel; }
   CSourceFileModel*    getSourceFileModel()    { return m_pSourceFileModel; }
   CTileStampModel*     getTileStampModel()     { return m_pTileStampModel; }

private:
   CNesicideProject*    m_pProject;

   CAttributeModel*     m_pAttributeModel;
   CBinaryFileModel*    m_pBinaryFileModel;
   CCartridgeModel*     m_pCartridgeModel;
   CFilterModel*        m_pFilterModel;
   CGraphicsBankModel*  m_pGraphicsBankModel;
   CSourceFileModel*    m_pSourceFileModel;
   CTileStampModel*     m_pTileStampModel;
};

#endif // CPROJECTMODEL_H
