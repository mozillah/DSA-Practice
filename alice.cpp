#include <ctime>
#include <iostream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <dirent.h>
#include <jsoncpp/json/json.h> 
// #include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;


bool dirExists(const std::string& dirName_in)
{
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!

  return false;    // this is not a directory!
}
void Json_Load(string jsondata){

    string folder_name =".";
    std::time_t result = std::time(nullptr);
    std::cout << std::asctime(std::localtime(&result))  << result << " seconds since the Epoch\n";
    auto tm = *std::localtime(&result);
    std::cout  <<"today"<< put_time(&tm, "%d-%m-%Y") << std::endl;
    cout << to_string(result) << "000" << endl;
    string date_time_obj = to_string(put_time(&tm, "%d-%m-%Y")) ;
    print(folder_name);
    string file_name ="regionsCount.json";
    string path = folder_name +"/"+ date_time_obj;

    int check;
  
    // check if directory is created or not
    
    if (dirExists(path)){
        check = mkdir(path,0777);
        if (!check)
        printf("Directory created\n");
        else {
        printf("Unable to create directory\n");
        // // exit(1);
    }
    }
     
    string directFile = folder_name+"/"+date_time_obj+"/"+file_name;
    string data;

    // if os.path.exists(directFile):
    //         append_write = ",["+str(frame_time)+"000,"+json.dumps(class_)+"]"
    //     else:
    //         append_write = "[["+str(frame_time)+"000,"+json.dumps(class_)+"]"

    std::ofstream outfile;

    if(std::filesystem::exists(directFile)){

        data = ",[" + to_string(result) << "000," +jsondata +"]"
        // data = ",["+str(frame_time)+"000,"+json.dumps(class_)+"]";
    }else{
        data = "[[" + to_string(result) << "000," +jsondata +"]"
        // data = "[["+str(frame_time)+"000,"+json.dumps(class_)+"]"
    }
    outfile.open(directFile, std::ios_base::app); // append instead of overwrite
    outfile << data; 
        // os.makedirs(self.folder_name+"/"+date_time_obj)
    // file_name = "IP-{:03d}-PC-{}-1200-01.json".format(self.ip_num,date_time_obj)

}

int main() {

    
    Json_Load();
              
              
    // std::time_t t = std::time(0);  // t is an integer type
    // std::cout << t << " seconds since 01-Jan-1970\n";
    // cout << to_string(t) << "000" << endl;
    ifstream ifs("alice.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj); // reader can also read strings
    cout << "Book: " << obj["book"].asString() << endl;
    cout << "Year: " << obj["year"].asUInt() << endl;
    const Json::Value& characters = obj["characters"]; // array of characters
    for (int i = 0; i < characters.size(); i++){
        cout << "    name: " << characters[i]["name"].asString();
        cout << " chapter: " << characters[i]["chapter"].asUInt();
        cout << endl;
    }
}