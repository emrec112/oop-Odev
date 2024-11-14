#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

class Student{
    private:
    string *studentNames;
    string *studentIDs;
    float *midterm;
    float *secondExam;
    float *assignment;
    float *final;
    int *numberOfAttendance;
    int studentCount;//öğrenci sayısını tutar

    public:
        //constuctor
        Student(int number){//öğrenci sayısınca diziler olsturur
            studentCount = number;
            studentNames = new string[studentCount];
            studentIDs = new string[studentCount];
            midterm = new float[studentCount];
            secondExam = new float[studentCount];
            assignment = new float[studentCount];
            final = new float[studentCount];
            numberOfAttendance = new int[studentCount];
        }
        //destructor
        ~Student(){
        delete[] studentNames;
        delete[] studentIDs;
        delete[] midterm;
        delete[] secondExam;
        delete[] assignment;
        delete[] final;
        delete[] numberOfAttendance;
        }

        void readFromCSV(){//dosyadan öğrencilerin bilgisini okur ve onları ilgili dizilerine kaydeder
            ifstream datas("data.csv");
            string str;
            getline(datas, str);//ilk satırı atlar
            for(int i = 0; i < studentCount; i++){
                getline(datas, str);//bütün satırı alır
                stringstream tempstring(str);//Alınan stringi akış nesnesine dönüştürerek üzerinde bölme parçalama
                //seçme işlemlerini yapmamıza olanak sağlar.

                getline(tempstring, studentNames[i], ',');
                getline(tempstring, studentIDs[i], ',');
                tempstring >> midterm[i];
                tempstring.ignore(1);//virgülü atlar 
                tempstring >> secondExam[i];
                tempstring.ignore(1); 
                tempstring >> assignment[i];
                tempstring.ignore(1); 
                tempstring >> final[i];
                tempstring.ignore(1);

                if(tempstring.peek() == ',' || tempstring.eof())//Stringstream ile tek bir satır alınan string akışa
                //çevrildikten sonra devam sayısının olduğu satırda sayı yazmıyorsa son eleman doğal olarak ','
                //olacak. Son eleman ',' ise 0 değilse o değeri devam sayısına koyacak.
                    numberOfAttendance[i] = 0;
                else{//varsa
                    int att;
                    tempstring >> att;
                    numberOfAttendance[i] = att;
                }
            }
            datas.close();
        }

        float average(int n){//ortalama hesaplar
            return midterm[n] * 0.2 + secondExam[n] * 0.2 + assignment[n] * 0.2
                + final[n] * 0.2;
        }

        void print() const{
            cout << setw(10) << "Isim" << setw(15) << "Numara" << setw(15) << "Ara Sinav" << setw(15) << "2. Sinav" 
             << setw(15) << "Odev" << setw(15) << "Final" << setw(20) << "Devam Sayisi" << endl;

            for(int i = 0; i < studentCount - 1; i++){
                 cout << setw(10) << studentNames[i] << setw(15) << studentIDs[i] << setw(15) << midterm[i] << setw(15) 
                 << secondExam[i]<< setw(15) << assignment[i] << setw(15) << final[i] << setw(20) << numberOfAttendance[i] << endl;
            }
        }

        void print(int choice){

            cout << setw(10) << "Isim" << setw(15) << "Numara" << setw(15) << "Ara Sinav" << setw(15) << "2. Sinav" 
             << setw(15) << "Odev" << setw(15) << "Final" << setw(20) << "Devam Sayisi" << endl;

            for(int i = 0; i < studentCount - 1; i++){

                if((average(i)) <= 50 && choice == 1)//geçenlerden 50 ve 50'den aşağısını alma
                    continue;
                else if((average(i)) > 50 && choice == 0)//kalanlardan 50 yukarısını alma
                    continue;
                
                 cout << setw(10) << studentNames[i] << setw(15) << studentIDs[i] << setw(15) << midterm[i] << setw(15) 
                 << secondExam[i]<< setw(15) << assignment[i] << setw(15) << final[i] << setw(20) << numberOfAttendance[i] << endl;
            }
        }

         void print(const string &fname){
            ofstream printfile(fname + ".csv");

            printfile << setw(10) << "Isim" << setw(15) << "Numara" << setw(15) << "Ara Sinav" << setw(15) << "2. Sinav" 
             << setw(15) << "Odev" << setw(15) << "Final" << setw(20) << "Devam Sayisi" << endl;

            for(int i = 0; i < studentCount - 1; i++){//dosyaya aktar
                 printfile << setw(10) << studentNames[i] << setw(15) << studentIDs[i] << setw(15) << midterm[i] << setw(15) 
                 << secondExam[i]<< setw(15) << assignment[i] << setw(15) << final[i] << setw(20) << numberOfAttendance[i] << endl;
            }
        }
};

int main() {

    ifstream file("data.csv");
    int NumberOfStudents = 0;//okunan öğrencilerin sayısını başta alıyoruz
    string str;
    getline(file, str);//ilk satırı atlar
    while(!file.eof()){
        getline(file, str);
        NumberOfStudents++;
    }
    file.close();
    Student students(NumberOfStudents);//öğrenci sayısını parametre olarak alarak constructorda yeterince yer açar.
    
    while(true){
        cout << "Kalan ogrencilerin bilgisi icin (0)\n"
        << "Gecen ogrencilerin bilgisi icin (1)\n"
        << "Tum ogrencileri görmek icin (2)\n"
        << "Ogrenci bilgilerini dosyaya atmak icin (3)\n"
        << "Programdan cikis icin (-1) : ";
        
        int choice;
        cin >> choice;
        if(choice == -1)
            break;
        
        students.readFromCSV();//öğrencileri burada kaydediyor
        
        switch(choice){
            case 0:
                students.print(choice);//kalanlar
                break;
            case 1:
                students.print(choice);//geçenler
                break;
            case 2:
                students.print();//tüm öğrenciler
                break;
            case 3:
                cin.ignore();
                string FileName;
                getline(cin, FileName);
                students.print(FileName);//dosya ismi ile tüm öğrencileri kaydet
                cout << "dosya basarili bir sekilde olusturuldu.\n";
                break;
        }
        
    }
    return 0;
}
