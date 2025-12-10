#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <chrono>

using namespace std;

struct Soal {
    string pertanyaan;
    string a, b, c, d, e;
    char kunciJawaban;
    string pembahasan;
};

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<Soal> loadSoal(string namaFile) {
    vector<Soal> daftarSoal;
    ifstream file(namaFile);
    string baris;

    if (!file.is_open()) {
        return daftarSoal;
    }

    while (getline(file, baris)) {
        if (baris.empty()) continue;

        vector<string> data = split(baris, '#');

        if (data.size() >= 8) {
            Soal s;
            s.pertanyaan = data[0];
            s.a = data[1];
            s.b = data[2];
            s.c = data[3];
            s.d = data[4];
            s.e = data[5];
            s.kunciJawaban = data[6][0];
            s.pembahasan = data[7];
            daftarSoal.push_back(s);
        }
    }
    file.close();
    return daftarSoal;
}

void kerjakanSubtes(string namaSubtes, vector<Soal> daftarSoal) {
    if (daftarSoal.empty()) {
        cout << "\n[ERROR] Soal tidak ditemukan untuk " << namaSubtes << endl;
        cout << "Pastikan file .txt tersedia dan formatnya benar." << endl;
        cout << "Tekan Enter untuk kembali...";
        cin.ignore();
        cin.get();
        return;
    }

    int skor = 0;
    char jawab;
    const double BATAS_WAKTU = 60.0;

    clearScreen();
    cout << "==========================================" << endl;
    cout << " SUBTES: " << namaSubtes << endl;
    cout << " Jumlah Soal: " << daftarSoal.size() << endl;
    cout << " Waktu: " << BATAS_WAKTU << " detik per soal" << endl;
    cout << "==========================================" << endl;

    for (int i = 0; i < daftarSoal.size(); i++) {
        cout << "\n[Soal " << i + 1 << "]" << endl;
        cout << daftarSoal[i].pertanyaan << endl;
        cout << "A. " << daftarSoal[i].a << endl;
        cout << "B. " << daftarSoal[i].b << endl;
        cout << "C. " << daftarSoal[i].c << endl;
        cout << "D. " << daftarSoal[i].d << endl;
        cout << "E. " << daftarSoal[i].e << endl;

        auto start = chrono::steady_clock::now();

        cout << ">>> Jawaban (a/b/c/d/e): ";
        cin >> jawab;

        auto end = chrono::steady_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;
        double durasi = elapsed_seconds.count();

        jawab = tolower(jawab);

        if (durasi > BATAS_WAKTU) {
            cout << "WAKTU HABIS! Kamu menghabiskan " << fixed << setprecision(1) << durasi << " detik." << endl;
            cout << "Jawaban tidak dihitung." << endl;
            cout << "Kunci: " << (char)toupper(daftarSoal[i].kunciJawaban) << endl;
        } 
        else if (jawab == daftarSoal[i].kunciJawaban) {
            cout << "BENAR! (" << fixed << setprecision(1) << durasi << " detik)" << endl;
            skor++;
        } else {
            cout << "SALAH. Kunci: " << (char)toupper(daftarSoal[i].kunciJawaban) << endl;
        }
        
        cout << "------------------------------------------" << endl;
        cout << "PEMBAHASAN: " << daftarSoal[i].pembahasan << endl;
        cout << "------------------------------------------" << endl;
    }

    double nilaiAkhir = ((double)skor / daftarSoal.size()) * 1000;

    cout << "\n==========================================" << endl;
    cout << " HASIL AKHIR " << namaSubtes << endl;
    cout << " Benar: " << skor << " / " << daftarSoal.size() << endl;
    cout << " Skor UTBK: " << fixed << setprecision(0) << nilaiAkhir << endl;
    cout << "==========================================" << endl;
    cout << "Tekan Enter untuk kembali ke menu utama...";
    cin.ignore();
    cin.get();
}

int main() {
    int pilihan;

    do {
        clearScreen();
        cout << "==========================================" << endl;
        cout << "   PLATFORM SIMULASI UTBK SNBT (SDGs 4)" << endl;
        cout << "==========================================" << endl;
        
        cout << "--- TES POTENSI SKOLASTIK (TPS) ---" << endl;
        cout << "1. Penalaran Umum (PU)" << endl;
        cout << "2. Pengetahuan & Pemahaman Umum (PPU)" << endl;
        cout << "3. Pemahaman Bacaan & Menulis (PBM)" << endl;
        cout << "4. Pengetahuan Kuantitatif (PK)" << endl;
        
        cout << "\n--- TES LITERASI ---" << endl;
        cout << "5. Literasi Bahasa Indonesia" << endl;
        cout << "6. Literasi Bahasa Inggris" << endl;
        cout << "7. Penalaran Matematika (PM)" << endl;
        
        cout << "\n0. Keluar Aplikasi" << endl;
        cout << "==========================================" << endl;
        cout << "Pilih Subtes (0-7): ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                kerjakanSubtes("Penalaran Umum", loadSoal("soal_pu.txt"));
                break;
            case 2:
                kerjakanSubtes("Pengetahuan & Pemahaman Umum", loadSoal("soal_ppu.txt"));
                break;
            case 3:
                kerjakanSubtes("Pemahaman Bacaan & Menulis", loadSoal("soal_pbm.txt"));
                break;
            case 4:
                kerjakanSubtes("Pengetahuan Kuantitatif", loadSoal("soal_pk.txt"));
                break;
            case 5:
                kerjakanSubtes("Literasi B. Indonesia", loadSoal("soal_lit_indo.txt"));
                break;
            case 6:
                kerjakanSubtes("Literasi B. Inggris", loadSoal("soal_lit_ing.txt"));
                break;
            case 7:
                kerjakanSubtes("Penalaran Matematika", loadSoal("soal_pm.txt"));
                break;
            case 0:
                cout << "\nTerima kasih telah belajar! Sukses UTBK!" << endl;
                break;
            default:
                cout << "\nPilihan tidak valid. Silakan coba lagi." << endl;
                cin.ignore();
                cin.get();
                break;
        }

    } while (pilihan != 0);

    return 0;
}