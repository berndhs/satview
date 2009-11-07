#

cd ~/public_html/daily/

rm ECIR.JPG
wget http://www.goes.noaa.gov/GIFS/ECIR.JPG 
ls -l ECIR*
pwd
php ~/scripts/storeIR.php ~/public_html/daily/ECIR.JPG

rm WCIR.JPG
wget http://www.goes.noaa.gov/GIFS/WCIR.JPG
ls -l WCIR*
pwd
php ~/scripts/storeIR.php ~/public_html/daily/WCIR.JPG