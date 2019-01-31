#./waf --run "IOT_ABE --MU=2 --SD=3"
./waf --run "IOT_ABE --MU=2 --SD=5"
./waf --run "IOT_ABE --MU=2 --SD=10"
./waf --run "IOT_ABE --MU=2 --SD=15"
./waf --run "IOT_ABE --MU=2 --SD=20"


./waf --run "IOT_ABE --MU=3 --SD=3"
./waf --run "IOT_ABE --MU=3 --SD=5"
./waf --run "IOT_ABE --MU=3 --SD=10"
./waf --run "IOT_ABE --MU=3 --SD=15"
./waf --run "IOT_ABE --MU=3 --SD=20"


./waf --run "IOT_ABE --MU=5 --SD=3"
./waf --run "IOT_ABE --MU=5 --SD=5"
./waf --run "IOT_ABE --MU=5 --SD=10"
./waf --run "IOT_ABE --MU=5 --SD=15"
./waf --run "IOT_ABE --MU=5 --SD=20"

./waf --run "IOT_ABE --MU=8 --SD=3"
./waf --run "IOT_ABE --MU=8 --SD=5"
./waf --run "IOT_ABE --MU=8 --SD=10"
./waf --run "IOT_ABE --MU=8 --SD=15"
./waf --run "IOT_ABE --MU=8 --SD=20"


./waf --run "IOT_ABE --MU=10 --SD=3"
./waf --run "IOT_ABE --MU=10 --SD=5"
./waf --run "IOT_ABE --MU=10 --SD=10"
./waf --run "IOT_ABE --MU=10 --SD=15"
./waf --run "IOT_ABE --MU=10 --SD=20"
./waf --run "IOT_ABE --MU=10 --SD=35"
./waf --run "IOT_ABE --MU=10 --SD=50"



python xmlParser.py


cp summary.csv dropbox/



./waf --run "IOT_ABE --MU=2 --SD=35"
./waf --run "IOT_ABE --MU=2 --SD=50"


./waf --run "IOT_ABE --MU=3 --SD=35"
./waf --run "IOT_ABE --MU=3 --SD=50"


./waf --run "IOT_ABE --MU=5 --SD=35"
./waf --run "IOT_ABE --MU=5 --SD=50"


./waf --run "IOT_ABE --MU=8 --SD=35"
./waf --run "IOT_ABE --MU=8 --SD=50"


./waf --run "IOT_ABE --MU=10 --SD=35"
./waf --run "IOT_ABE --MU=10 --SD=50"




#python filter.py

#python calculate.py >> summary.csv

python xmlParser.py


cp summary.csv dropbox/
