#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// personType sınıfı
class personType {
public:

    void setName(string first, string last);
    string getName()const;
    string getLastName() const;
    personType(string first = "", string last = "");

private:
    string firstName;
    string lastName;
};

// courseType sınıfı
class courseType {
private:
    string courseName;
    string courseNumber;
    int courseHours;
    char courseGrade;
    class studentType* student;  // studentType ile birleşim (composition)

public:

    void setCourseInfo(string cName, string cNo, int cHours, char cGrade, class studentType* s);
    string getCourseName() const;
    string getCourseNumber() const;
    int getCourseHours() const;
    char getCourseGrade() const;
    courseType(string cName = "", string cNo = "", int cHours = 0, char cGrade = 'F', class studentType* s = nullptr);

    friend istream& operator>>(istream& in, courseType& course);
    friend ostream& operator<<(ostream& out, const courseType& course);
};

// studentType sınıfı
class studentType : public personType {
private:
    int studentID;
    bool isTuitionPaid;
    courseType** courses;  // Dinamik dizi
    int numCourses;

public:
    studentType(string first = "", string last = "", int id = 0, bool tuitionPaid = false);
    ~studentType();  // Destructor
    void setStudentInfo(string first, string last, int id, bool tuitionPaid);
    void addCourse(courseType* course);
    void printStudentInfo(ostream& out) const;
    int totalCredits() const;
    float calculateGPA() const;

    friend ostream& operator<<(ostream& out, const studentType& student);
};

// personType sınıfı metodları
personType::personType(string first, string last) {
    setName(first, last);
}

void personType::setName(string first, string last) {
    firstName = first;
    lastName = last;
}

string personType::getName() const {
    return firstName;
}

string personType::getLastName() const {
    return lastName;
}


// courseType sınıfı metodları
void courseType::setCourseInfo(string cName, string cNo, int cHours, char cGrade, studentType* s) {
    courseName = cName;
    courseNumber = cNo;
    courseHours = cHours;
    courseGrade = cGrade;
    student = s;
}

courseType::courseType(string cName, string cNo, int cHours, char cGrade, studentType* s) {
    setCourseInfo(cName, cNo, cHours, cGrade, s);
}



string courseType::getCourseName() const {
    return courseName;
}

string courseType::getCourseNumber() const {
    return courseNumber;
}

int courseType::getCourseHours() const {
    return courseHours;
}

char courseType::getCourseGrade() const {
    return courseGrade;
}

istream& operator>>(istream& in, courseType& course) {
    in >> course.courseName >> course.courseNumber >> course.courseHours >> course.courseGrade;
    return in;
}

ostream& operator<<(ostream& out, const courseType& course) {
    out << course.courseName << " " << course.courseNumber << " " << course.courseHours << " " << course.courseGrade;
    return out;
}

// studentType sınıfı metodları
studentType::studentType(string first, string last, int id, bool tuitionPaid) {
    setStudentInfo(first, last, id, tuitionPaid);
    courses = nullptr;
    numCourses = 0;
}

studentType::~studentType() {
    for (int i = 0; i < numCourses; ++i) {
        delete courses[i];  // courseType nesnelerini sil
    }
    delete[] courses;  // kurs dizisini sil
}

void studentType::setStudentInfo(string first, string last, int id, bool tuitionPaid) {
    setName(first, last);  // personType sınıfından miras alınan fonksiyon
    studentID = id;
    isTuitionPaid = tuitionPaid;
}

void studentType::addCourse(courseType* course) {
    courseType** temp = new courseType*[numCourses + 1];  // Yeni kurs sayısına göre dizi boyutu arttır
    for (int i = 0; i < numCourses; ++i) {
        temp[i] = courses[i];  // Mevcut dersleri yeni diziye kopyala
    }
    temp[numCourses] = course;  // Yeni ders ekle
    delete[] courses;  // Eski kurs dizisini sil
    courses = temp;  // Yeni kurs dizisini atama
    numCourses++;  // Ders sayısını arttır
}

void studentType::printStudentInfo(ostream& out) const {
    out << "Isim: "<<this->getName()<<" "<<this->getLastName()<<endl;

    out << "Numara: " << studentID << endl;

    if (isTuitionPaid) {
        out << "Alinan Ders Sayisi: " << numCourses << endl;
        out << "Ogrencinin Ders Bilgileri:" << endl;

        int totalCredits = 0;
        float totalPoints = 0;

        for (int i = 0; i < numCourses; ++i) {
            out << courses[i]->getCourseNumber() << " " << courses[i]->getCourseName() << " " << courses[i]->getCourseHours() << " " << courses[i]->getCourseGrade() << endl;
            totalCredits += courses[i]->getCourseHours();
            switch (courses[i]->getCourseGrade()) {
                case 'A': totalPoints += 4.0 * courses[i]->getCourseHours(); break;
                case 'B': totalPoints += 3.0 * courses[i]->getCourseHours(); break;
                case 'C': totalPoints += 2.0 * courses[i]->getCourseHours(); break;
                case 'D': totalPoints += 1.0 * courses[i]->getCourseHours(); break;
                case 'F': totalPoints += 0.0 * courses[i]->getCourseHours(); break;
            }
        }

        out << "Derslerin toplam kredisi: " << totalCredits << endl;

        float GPA = totalPoints / totalCredits;
        out << " GPA: " << GPA << endl;
    } else {
        out << "Harc Odenmedi!!" << endl;
    }
}

int studentType::totalCredits() const {
    int total = 0;
    for (int i = 0; i < numCourses; ++i) {
        total += courses[i]->getCourseHours();
    }
    return total;
}

float studentType::calculateGPA() const {
    int totalCredits = this->totalCredits();
    float totalPoints = 0;

    for (int i = 0; i < numCourses; ++i) {
        switch (courses[i]->getCourseGrade()) {
            case 'A': totalPoints += 4.0 * courses[i]->getCourseHours(); break;
            case 'B': totalPoints += 3.0 * courses[i]->getCourseHours(); break;
            case 'C': totalPoints += 2.0 * courses[i]->getCourseHours(); break;
            case 'D': totalPoints += 1.0 * courses[i]->getCourseHours(); break;
            case 'F': totalPoints += 0.0 * courses[i]->getCourseHours(); break;
        }
    }

    return totalCredits > 0 ? totalPoints / totalCredits : 0.0;
}

ostream& operator<<(ostream& out, const studentType& student) {
    student.printStudentInfo(out);
    return out;
}

// main fonksiyonu
int main() {
    ifstream inputFile("dosya.txt");  // Girdi dosyası
    ofstream outputFile("output.txt"); // Çıktı dosyası

    if (!inputFile) {
        return 1;
    }

    int numStudents, feePerCredit;
    inputFile >> numStudents >> feePerCredit;

    bool firstStudent = true;  // İlk öğrencinin yazıldığı durum
    for (int i = 0; i < numStudents; ++i) {
        string firstName, lastName, tuitionStatus;
        int studentID, numCourses;

        inputFile >> firstName >> lastName >> studentID >> tuitionStatus >> numCourses;
        bool tuitionPaid = (tuitionStatus == "Y");

        studentType student(firstName, lastName, studentID, tuitionPaid);

        for (int j = 0; j < numCourses; ++j) {
            string courseName, courseNumber;
            int courseHours;
            char courseGrade;

            inputFile >> courseName >> courseNumber >> courseHours >> courseGrade;

            courseType* course = new courseType(courseName, courseNumber, courseHours, courseGrade, &student);
            student.addCourse(course);
        }

        if (!firstStudent) {
            outputFile << "----------------------------" << endl;
        }
        firstStudent = false;

        outputFile << student;
    }

    cout<<"Bilgiler output.txt'ye yazildi"<<endl;
    outputFile.close();
    inputFile.close();

    return 0;
}
