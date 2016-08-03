#include "NcLoader.hpp"

#include <QDebug>
#include <cassert>

NcLoader::NcLoader()
{

}

NcLoader::~NcLoader()
{
    delete ncFile;
}

void NcLoader::loadNCFile(QString fp)
{
    this->filepath = fp;

    qDebug() << "Loading file" << this->filepath;
    this->ncFile = new netCDF::NcFile(this->filepath.toStdString(), netCDF::NcFile::read);
    if(this->ncFile->isNull()) {
        qDebug() << "Could not load file" << this->filepath;
        assert(0);
    }
}

void NcLoader::getDataFromNC(QString var, std::vector<size_t> start, std::vector<size_t> count, std::vector<float> &values)
{
    netCDF::NcVar tempVar = this->ncFile->getVar(var.toStdString());
    if(tempVar.isNull()) {
        qDebug() << "Could not read variable" << var;
        return;
    }

    int size = 1;
    for(int i=0; i<count.size(); i++) {
        size *= count[i];
    }
    values.resize(size);

    qDebug() << "Reading variable" << var << "with dimensions:";
    std::vector<netCDF::NcDim> dims = tempVar.getDims();
    for(int i=0; i<dims.size(); i++) {
        qDebug() << QString::fromStdString(dims[i].getName())
                 << dims[i].getSize();
    }

    tempVar.getVar(start, count, values.data());

}

int NcLoader::getSize(QString dim)
{
    return this->ncFile->getDim(dim.toStdString()).getSize();
}

void NcLoader::printInfo()
{
    // print info
    qDebug() << "==============";
    qDebug() << "attributes:";
    std::multimap<std::string,netCDF::NcGroupAtt> atts = this->ncFile->getAtts();
    for (std::multimap<std::string,netCDF::NcGroupAtt>::iterator it=atts.begin(); it!=atts.end(); ++it) {
        std::string name = (*it).first;
        qDebug() << QString::fromStdString(name) << this->ncFile->getAtt(name).getAttLength();
    }

    qDebug() << "==============";
    qDebug() << "dimensions:";
    std::multimap<std::string,netCDF::NcDim> dims = this->ncFile->getDims();
    for (std::multimap<std::string,netCDF::NcDim>::iterator it=dims.begin(); it!=dims.end(); ++it) {
        std::string name = (*it).first;
        qDebug() << QString::fromStdString(name) << this->ncFile->getDim(name).getSize();
    }

    qDebug() << "==============";
    qDebug() << "variables:";
    std::multimap<std::string,netCDF::NcVar> vars = this->ncFile->getVars();
    for (std::multimap<std::string,netCDF::NcVar>::iterator it=vars.begin(); it!=vars.end(); ++it) {
        std::string name = (*it).first;
        qDebug() << QString::fromStdString(name) << QString::fromStdString(this->ncFile->getVar(name).getType().getName());
    }

    qDebug() << "==============";
    qDebug() << "types:";
    std::multimap<std::string,netCDF::NcType> types = this->ncFile->getTypes();
    for (std::multimap<std::string,netCDF::NcType>::iterator it=types.begin(); it!=types.end(); ++it) {
        std::string name = (*it).first;
        qDebug() << QString::fromStdString(name);
    }

    qDebug() << "==============";
    qDebug() << "groups:";
    std::multimap<std::string,netCDF::NcGroup> groups = this->ncFile->getGroups();
    for (std::multimap<std::string,netCDF::NcGroup>::iterator it=groups.begin(); it!=groups.end(); ++it) {
        std::string name = (*it).first;
        qDebug() << QString::fromStdString(name);
    }
    qDebug() << "==============";
}

