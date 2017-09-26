textan.cgi: freqan.o stats.o
    g++ -static -s -o textan.cgi freqan.o stats.o
    
freqan.o: freqan.cpp stats.hpp
    g++ -O2 -c freqan.cpp
    
stats.o: stats.cpp stats.hpp
    g++ -O2 -c stats.cpp
    
install:
    sudo cp textan.cgi /srv/httpd/cgi-bin
    sudo chown apache:apache /srv/httpd/cgi-bin/textan.cgi
    
clean:
    rm textan.cgi && rm *.o
