//
// 2024-2025 KTÜ Bilgisayar Mühendisliği
// Object Oriented Programming Dönem Ödevi
//
// 434377 - Deniz Tunç
// 439553 - Emre Camuzoğlu
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <string.h>
using namespace std;

#define SEP setw(0) << " |"

enum Choice {
  ch_exit = 0,
  ch_all = 1,
  ch_kalanlar = 2,
  ch_gecenler = 3,
  ch_dosya = 4,
};

// https://en.cppreference.com/w/cpp/language/operators#Stream_extraction_and_insertion
std::istream& operator>>(std::istream& in, Choice& choice) {
    int value;
    in >> value; // Girdiyi oku
    choice = static_cast<Choice>(value); // Enuma dönüştür
    return in;
}

const int DURUM_KALDI = 0;
const int DURUM_GECTI = 1;

const int DURUM_HEPSI = 2;

// `setw` değerleri
const std::_Setw cols[] = { 
  setw(strlen("Numara") + 1),
  setw(strlen("Ara Sinav") + 2),
  setw(strlen("2. Sinav") + 2),
  setw(strlen("Ödev") + 2),
  setw(strlen("Final") + 2),
  setw(strlen("Devam") + 2),
  setw(strlen("Durum") + 2),
  setw(2), // İsimden önceki boşluk
};

// # Öğrenci Listesi
class Student{
    private:
      string *studentNames;
      string *studentIDs;
      float *midterm;
      float *secondExam;
      float *assignment;
      float *finalExam;
      int *numberOfAttendance;
      int studentCount;//öğrenci sayısını tutar


      void print_row(int i, std::ostream& output) const {
        output
          << cols[0] << studentIDs[i] << SEP
          << cols[1] << midterm[i] << SEP
          << cols[2] << secondExam[i] << SEP
          << cols[3] << assignment[i] << SEP
          << cols[4] << finalExam[i] << SEP
          << cols[5] << numberOfAttendance[i] << SEP
          << cols[6] << ((average(i) <= 50) ? "Kaldi" : "Gecti") << SEP
          << left << cols[7] << "" << studentNames[i] << right
          << endl;
      }

      static void print_header(ostream& output) {
        output
          << cols[0] << "Numara" << SEP
          << cols[1] << "Ara Sinav" << SEP
          << cols[2] << "2. Sinav" << SEP
          << cols[3] << "Odev" << SEP
          << cols[4] << "Final" << SEP
          << cols[5] << "Devam" << SEP 
          << cols[6] << "Durum" << SEP
          << left << cols[7] << "" << "Isim" << right
          << endl;
      }

    public:
        // Constuctor
        Student(int studentCount){
            this->studentCount = studentCount;

            // Dizileri öğrenci sayısınca dinamik şekilde oluşturur
            studentNames = new string[studentCount];
            studentIDs = new string[studentCount];
            midterm = new float[studentCount];
            secondExam = new float[studentCount];
            assignment = new float[studentCount];
            finalExam = new float[studentCount];
            numberOfAttendance = new int[studentCount];
        }
        //destructor
        ~Student(){
          delete[] studentNames;
          delete[] studentIDs;
          delete[] midterm;
          delete[] secondExam;
          delete[] assignment;
          delete[] finalExam;
          delete[] numberOfAttendance;
        }

        // Dosyadan öğrencilerin bilgisini okur ve onları ilgili dizilerine kaydeder
        void readFromCSV(){
            ifstream datas("data.csv");
            string str;
            getline(datas, str);//ilk satırı atlaro
            for(int i = 0; i < studentCount; i++){
                // Bütün satırı okur
                getline(datas, str);

                // Alınan stringi akış nesnesine dönüştürerek üzerinde bölme parçalama
                // seçme işlemlerini yapmamıza olanak sağlar.
                stringstream tempstring(str);

                // Virgüle kadar olan stringi alır.
                getline(tempstring, studentNames[i], ',');
                getline(tempstring, studentIDs[i], ',');
                tempstring >> midterm[i];
                tempstring.ignore(1); // virgül
                tempstring >> secondExam[i];
                tempstring.ignore(1); 
                tempstring >> assignment[i];
                tempstring.ignore(1); 
                tempstring >> finalExam[i];
                tempstring.ignore(1);

                // Stringstream ile tek bir satır alınan string akışa çevrildikten
                // sonra devam sayısının olduğu satırda sayı yazmıyorsa son eleman
                // doğal olarak ',' olacak.
                // Son eleman ',' ise 0, değilse o değeri devam sayısına koyacak.
                
                // Yoklama verisi yoksa
                if(tempstring.peek() == ',' || tempstring.eof()) {
                    numberOfAttendance[i] = 0;
                }
                else{
                    int att;
                    tempstring >> att;
                    numberOfAttendance[i] = att;
                }
            }
            datas.close();
        }

        // Öğrenci sınav ortalaması
        float average(int i) const {
            return (
								midterm[i] +
								secondExam[i] +
								assignment[i] +
								finalExam[i] * 2
							) / 5;
        }

				// Öğrencileri ekrana yazdırır.
        void print() const{

          print_header(cout);

          for(int i = 0; i < studentCount - 1; i++){
            print_row(i, cout);
          }
        }

				// Kalan/Geçen öğrencileri ekrana yazdırır.
        void print(int durum){

          print_header(cout);

          for(int i = 0; i < studentCount - 1; i++){
            //geçenlerden 50 ve 50'den aşağısını alma
            if((average(i)) <= 50 && durum == DURUM_GECTI)
                continue;
            //kalanlardan 50 yukarısını alma
            else if((average(i)) > 50 && durum == DURUM_KALDI)
                continue;

            print_row(i, cout);
          }
        }


				// Kalan/Geçen/Tüm öğrencileri dosyaya yazdırır.
        int print(int durum, const string &fname){
            ofstream printfile(fname + ".txt");

            if (printfile.fail()) return -1; // FAIL

            print_header(printfile);

            for(int i = 0; i < studentCount - 1; i++){
              //geçenlerden 50 ve 50'den aşağısını alma
              if((average(i)) <= 50 && durum == DURUM_GECTI)
                  continue;
              //kalanlardan 50 yukarısını alma
              else if((average(i)) > 50 && durum == DURUM_KALDI)
                  continue;

              print_row(i, printfile);
            }
            return 0; // OK
        }
};

int main() {

    ifstream file("data.csv");
    // Okunacak öğrenci sayısını satır sayısına göre ayarlıyoruz.
    int student_count = 0;

    string tmp_line;

    getline(file, tmp_line);
    while(!file.eof()){
        getline(file, tmp_line);
        student_count++;
    }
    file.close();

    // Dizi boyutlarını öğrenci sayısına göre dinamik olarak ayarlıyoruz.

    // Öğrenci listesi
    Student students(student_count);
    
    while(true){
        cout 
          << "Tüm öğrencileri görmek için     (" << ch_all << ")" << endl
          << "Kalan ögrencilerin bilgisi için (" << ch_kalanlar << ")" << endl
          << "Gecen öğrencilerin bilgisi için (" << ch_gecenler << ")" << endl
          << "Ögrencileri dosyaya yazmak için (" << ch_dosya << ")" << endl
          << "Programdan çıkmak için          (" << ch_exit << ")" << endl
          << ":";
        
        enum Choice choice;
        cin >> choice;
        cin.ignore();
        
        students.readFromCSV(); //öğrencileri dosyadan oku
        
        switch(choice){
            case ch_kalanlar:
                students.print(DURUM_KALDI);
                break;
            case ch_gecenler:
                students.print(DURUM_GECTI);
                break;
            case ch_all:
                students.print();
                break;
            case ch_dosya: {
                int durum;
                string fileName;

                cout <<
                  "Kalanlar için 0" << endl <<
                  "Geçenler için 1" << endl <<
                  "Tüm liste için 2" << endl <<
                  ": ";
                cin >> durum;
                cin.ignore();

                cout << "Yazılacak dosya adı: ";
                getline(cin, fileName);

                int error = students.print(durum, fileName);

                if (error != 0)
                  cerr << "Dosya yazılırken bir hata oluştu!" << endl;
                else
                  cout << "Dosya başarılı bir şekilde oluşturuldu: \"" << fileName << ".txt\"" << endl;

                break;
            }
            case ch_exit:
                cout << "Çıkış..." << endl;
                exit(0); // OK
            default:
                cout << "Yanlış bir seçim girdiniz." << endl;
                break;
        }
        
    }
    return 0;
}

