#include <fstream>
#include <iostream>
#include <string>

int main(){
    std::fstream fs1("./render_log.txt");
    std::fstream fs2("./render_log_2.txt");
    std::fstream ofs("./cmp_res.txt");
    while(!(fs1.eof()&&fs2.eof())){
        std::string line1;
        std::string line2;
        std::getline(fs1,line1);
        std::getline(fs2,line2);
        if(line1!=line2)ofs<<"!!!\n"<<line1<<'\n'<<line2<<'\n';
    }
    fs1.close();
    fs2.close();
    ofs.close();
}