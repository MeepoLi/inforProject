#ifndef NCLOADER_H
#define NCLOADER_H

#include <netcdf>
#include <QString>

class NcLoader
{
public:
    NcLoader();
    ~NcLoader();
    void loadNCFile(QString filepath);
    void loadBinFile(QString filepath);
    void saveBinFile(QString filepath, std::vector<float>& values);
    void saveBinFile(QString variable, int timestep);
    void getDataFromNC(QString variable, std::vector<size_t> start, std::vector<size_t> count, std::vector<float>& values);
    void getDataFromBin(QString variable, int timestep, std::vector<float>& values);
    int getSize(QString dim);
    void printInfo();

private:
    QString filepath;
    netCDF::NcFile *ncFile;

};

#endif // NCLOADER_H
