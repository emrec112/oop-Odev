#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <string.h>
using namespace std;

#define SEP setw(0) << " |"

enum choice {
	ch_exit = 0,
	ch_all = 1,
	ch_kalanlar = 2,
	ch_gecenler = 3,
	ch_dosya = 4,
};

// `setw` değerleri
const _Setw cols[] = { 
	setw(strlen("Numara") + 1),
	setw(strlen("Ara Sınav") + 2),
	setw(strlen("2. Sınav") + 2),
	setw(strlen("Ödev") + 2),
	setw(strlen("Final") + 2),
	setw(strlen("Devam") + 2),
	setw(strlen("Durum") + 2),
	setw(15),
};

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

				// Dosyadan öğrencilerin bilgisini okur ve onları ilgili dizilerine kaydeder
        void readFromCSV(){
            ifstream datas("data.csv");
            string str;
            getline(datas, str);//ilk satırı atlar
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
                tempstring >> final[i];
                tempstring.ignore(1);

								// Stringstream ile tek bir satır alınan string akışa çevrildikten
								// sonra devam sayısının olduğu satırda sayı yazmıyorsa son eleman
								// doğal olarak ',' olacak.
								// Son eleman ',' ise 0 değilse o değeri devam sayısına koyacak.
								
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
            return (midterm[i] + secondExam[i] + assignment[i] + final[i]) / 4;
        }

				void print_row(int i, std::ostream& output) const {
					output
						<< cols[0] << studentIDs[i] << SEP
						<< cols[1] << midterm[i] << SEP
						<< cols[2] << secondExam[i] << SEP
						<< cols[3] << assignment[i] << SEP
						<< cols[4] << final[i] << SEP
						<< cols[5] << numberOfAttendance[i] << SEP
						<< cols[6] << ((average(i) < 50) ? "Kaldi" : "Gecti") << SEP
						<< cols[7] << studentNames[i]
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
						<< cols[7] << "Isim"
						<< endl;
				}

        void print() const{

					print_header(cout);

					for(int i = 0; i < studentCount - 1; i++){
						print_row(i, cout);
					}
        }

        void print(int choice){

					print_header(cout);

					for(int i = 0; i < studentCount - 1; i++){
						//geçenlerden 50 ve 50'den aşağısını alma
						if((average(i)) <= 50 && choice == choice::ch_gecenler)
								continue;
						//kalanlardan 50 yukarısını alma
						else if((average(i)) > 50 && choice == choice::ch_kalanlar)
								continue;

						print_row(i, cout);
					}
        }

         void print(const string &fname){
            ofstream printfile(fname + ".txt");

						print_header(printfile);

            for(int i = 0; i < studentCount - 1; i++){
							print_row(i, printfile);
            }
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
					<< "Tüm öğrencileri görmek için             (" << choice::ch_all << ")" << endl
					<< "Kalan ögrencilerin bilgisi için         (" << choice::ch_kalanlar << ")" << endl
					<< "Gecen öğrencilerin bilgisi için         (" << choice::ch_gecenler << ")" << endl
					<< "Ögrenci bilgilerini dosyaya yazmak için (" << choice::ch_dosya << ")" << endl
					<< "Programdan çıkmak için                  (" << choice::ch_exit << ")" << endl
					<< ":";
        
        enum choice choice;
        cin >> *((int *)&choice);
        if(choice == choice::ch_exit)
            break;
        
        students.readFromCSV();//öğrencileri burada kaydediyor
        
        switch(choice){
						case choice::ch_kalanlar:
                students.print(choice);
                break;
						case choice::ch_gecenler:
                students.print(choice);
                break;
						case choice::ch_all:
                students.print();
                break;
						case choice::ch_dosya: {
                cin.ignore();
                string FileName;
                getline(cin, FileName);
                students.print(FileName);//dosya ismi ile tüm öğrencileri kaydet
                cout << "Dosya başarılı bir şekilde oluşturuldu.\n";
                break;
						}
						default:
								cout << "Yanlış bir seçim girdiniz.\n";
								break;
        }
        
    }
    return 0;
}
