library(ncdf4)

setwd("/Users/fmiranda/Dropbox/vgc/ccny/datasets")
ncin<-nc_open("Discharge_NCEP_Pristine_dTS2004.nc")
print(ncin)
plot.ts(ncvar_get(ncin,"average"))