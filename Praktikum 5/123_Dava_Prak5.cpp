#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;

struct RelatedMovie {
    string nama;
    RelatedMovie* next;
};

struct GenreList {
    string nama;
    GenreList* next;
};

struct Movie {
    string nama;
    int episode;
    int season;
    string studio;

    float rating;
    int totalRating;

    GenreList* genreHead;
    RelatedMovie* relatedHead;

    Movie* left;
    Movie* right;
};

struct GenreMovie {
    Movie* movie;
    GenreMovie* next;
};

struct Genre {
    string nama;

    GenreMovie* movieHead;
    Genre* next;
};

Movie* root = NULL;

void saveToFile();
void loadFromFile();
void rateMovie(Movie* movie);

const int SIZE = 10;
Genre* hashTable[SIZE];

string genreList[] = {
    "Action",
    "Drama",
    "Fiction",
    "Comedy",
    "Animated",
};

int genreCount = 5;

int hashFunction(string genre) {
    int sum = 0;

    for (int i = 0; i < genre.length(); i++) {
        sum += genre[i];
    }

    return sum % SIZE;
}

Genre* findGenre(string nama) {
    int idx = hashFunction(nama);

    Genre* temp = hashTable[idx];

    while (temp) {
        if (temp->nama == nama) return temp;
        temp = temp->next;
    }

    return NULL;
}

void initGenre() {
    for (int i = 0; i < SIZE; i++) {
        hashTable[i] = NULL;
    }

    for (int i = 0; i < genreCount; i++) {
        Genre* newGenre = new Genre();

        newGenre->nama = genreList[i];
        newGenre->movieHead = NULL;
        newGenre->next = NULL;

        int idx = hashFunction(newGenre->nama);

        if (!hashTable[idx]) {
            hashTable[idx] = newGenre;
        } else {
            Genre* temp = hashTable[idx];

            while (temp->next) {
                temp = temp->next;
            }

            temp->next = newGenre;
        }
    }
}

Movie* createMovie(
    string nama,
    int episode,
    int season,
    string studio
) {
    Movie* newMovie = new Movie();

    newMovie->nama = nama;
    newMovie->episode = episode;
    newMovie->season = season;
    newMovie->studio = studio;

    newMovie->rating = 0;
    newMovie->totalRating = 0;

    newMovie->genreHead = NULL;
    newMovie->relatedHead = NULL;

    newMovie->left = NULL;
    newMovie->right = NULL;

    return newMovie;
}

Movie* insertMovie(Movie* root, Movie* newMovie) {
    if (!root) return newMovie;

    if (newMovie->nama < root->nama) {
        root->left = insertMovie(root->left, newMovie);
    }

    else if (newMovie->nama > root->nama) {
        root->right = insertMovie(root->right, newMovie);
    }

    return root;
}

string trimString(const string& value) {
    int start = 0;
    int end = value.length() - 1;

    while (start <= end && isspace((unsigned char)value[start])) start++;
    while (end >= start && isspace((unsigned char)value[end])) end--;

    if (start > end) return "";
    return value.substr(start, end - start + 1);
}

string toLowerCase(const string& value) {
    string result = value;
    transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return tolower(c);
    });
    return result;
}

Movie* searchMovie(Movie* root, string nama) {
    if (!root) return NULL;

    if (root->nama == nama) return root;

    if (nama < root->nama) {
        return searchMovie(root->left, nama);
    }

    return searchMovie(root->right, nama);
}

Movie* searchMoviePartial(Movie* root, const string& nama) {
    if (!root) return NULL;

    string lowerQuery = toLowerCase(nama);
    string lowerName = toLowerCase(root->nama);

    if (lowerName.find(lowerQuery) != string::npos || lowerQuery.find(lowerName) != string::npos) {
        return root;
    }

    Movie* found = searchMoviePartial(root->left, nama);
    if (found) return found;
    return searchMoviePartial(root->right, nama);
}

void collectMovies(Movie* root, bool filmsOnly, vector<Movie*>& movies) {
    if (!root) return;

    collectMovies(root->left, filmsOnly, movies);

    if (filmsOnly && root->episode == 0) {
        movies.push_back(root);
    }
    else if (!filmsOnly && root->episode > 0) {
        movies.push_back(root);
    }

    collectMovies(root->right, filmsOnly, movies);
}

void sortMoviesByRating(vector<Movie*>& movies) {
    sort(movies.begin(), movies.end(), [](Movie* a, Movie* b) {
        if (a->rating != b->rating) return a->rating > b->rating;
        return a->nama < b->nama;
    });
}

void rateMovieFromList(vector<Movie*>& movies) {
    int pilih;

    cout << "Pilih film: ";
    cin >> pilih;

    if (pilih < 1 || pilih > (int)movies.size()) {
        cout << "Pilihan tidak valid\n";
        return;
    }

    rateMovie(movies[pilih - 1]);
    saveToFile();
}

void addGenreToMovie(Movie* movie, string genre) {
    GenreList* newGenre = new GenreList();

    newGenre->nama = genre;
    newGenre->next = NULL;

    if (!movie->genreHead) {
        movie->genreHead = newGenre;
    }

    else {
        GenreList* temp = movie->genreHead;

        while (temp->next) {
            temp = temp->next;
        }

        temp->next = newGenre;
    }
}

void connectGenre(Movie* movie, string genreName) {
    Genre* genre = findGenre(genreName);

    if (!genre) {
        cout << "Genre " << genreName << " tidak terdaftar\n";
        return;
    }

    GenreMovie* newMovie = new GenreMovie();

    newMovie->movie = movie;
    newMovie->next = NULL;

    if (!genre->movieHead) {
        genre->movieHead = newMovie;
    }

    else {
        GenreMovie* temp = genre->movieHead;

        while (temp->next) {
            temp = temp->next;
        }

        temp->next = newMovie;
    }

    addGenreToMovie(movie, genreName);
}

void connectRelatedMovie(Movie* movie, string relatedName) {
    relatedName = trimString(relatedName);

    Movie* target = searchMovie(root, relatedName);
    if (!target) {
        target = searchMoviePartial(root, relatedName);
    }

    if (!target) {
        cout << "Film " << relatedName << " tidak dapat ditemukan\n";
        return;
    }

    RelatedMovie* newRelated = new RelatedMovie();

    newRelated->nama = target->nama;
    newRelated->next = NULL;

    if (!movie->relatedHead) {
        movie->relatedHead = newRelated;
    }

    else {
        RelatedMovie* temp = movie->relatedHead;

        while (temp->next) {
            temp = temp->next;
        }

        temp->next = newRelated;
    }
}

void displayMovie(Movie* movie) {
    if (!movie) return;

    cout << "---- " << movie->nama << " ----\n";

    cout << "Genre: ";

    GenreList* genreTemp = movie->genreHead;

    while (genreTemp) {
        cout << genreTemp->nama;

        if (genreTemp->next) cout << ", ";

        genreTemp = genreTemp->next;
    }

    cout << endl;

    if (movie->episode == 0) {
        cout << "Jumlah ep: -\n";
        cout << "Jumlah season: -\n";
    }

    else {
        cout << "Jumlah ep: " << movie->episode << endl;
        cout << "Jumlah season: " << movie->season << endl;
    }

    cout << "Film terkait:\n";

    RelatedMovie* relatedTemp = movie->relatedHead;

    while (relatedTemp) {
        cout << "- " << relatedTemp->nama << endl;
        relatedTemp = relatedTemp->next;
    }

    cout << "Studio: " << movie->studio << endl;
    cout << fixed << setprecision(1);
    cout << "Rating: " << movie->rating << endl;
}

void rateMovie(Movie* movie) {
    float newRate;

    cout << "Rating: ";
    cin >> newRate;

    float total = movie->rating * movie->totalRating;

    total += newRate;

    movie->totalRating++;

    movie->rating = total / movie->totalRating;

    cout << fixed << setprecision(1);
    cout << "Rating terbaru: " << movie->rating << endl;
}

void inorderFilm(Movie* root) {
    if (!root) return;

    inorderFilm(root->left);

    if (root->episode == 0) {
        cout << root->nama << " | " << root->rating << endl;
    }

    inorderFilm(root->right);
}

void inorderSeries(Movie* root) {
    if (!root) return;

    inorderSeries(root->left);

    if (root->episode > 0) {
        cout << root->nama << " | " << root->rating << endl;
    }

    inorderSeries(root->right);
}

void searchMenu() {
    string nama;

    cin.ignore();

    cout << "Nama: ";
    getline(cin, nama);

    Movie* movie = searchMovie(root, nama);

    if (!movie) {
        cout << "Film tidak ditemukan\n";
        return;
    }

    displayMovie(movie);

    char pilih;

    cout << "[C]lose [R]ate\n";
    cin >> pilih;

    if (pilih == 'R') {
        rateMovie(movie);
    }
}

void allFilmMenu() {
    cout << "==== Semua Film ====\n";

    vector<Movie*> movies;
    collectMovies(root, true, movies);

    if (movies.empty()) {
        cout << "Tidak ada film di database\n";
        return;
    }

    sortMoviesByRating(movies);

    for (int i = 0; i < (int)movies.size(); i++) {
        cout << i + 1 << ". " << movies[i]->nama << " | " << fixed << setprecision(1) << movies[i]->rating << "\n";
    }

    char pilih;
    cout << "[C]lose [R]ate\n";
    cin >> pilih;

    if (pilih == 'R' || pilih == 'r') {
        rateMovieFromList(movies);
    }
}

void allSeriesMenu() {
    cout << "==== Semua Series ====\n";

    vector<Movie*> movies;
    collectMovies(root, false, movies);

    if (movies.empty()) {
        cout << "Tidak ada series di database\n";
        return;
    }

    sortMoviesByRating(movies);

    for (int i = 0; i < (int)movies.size(); i++) {
        cout << i + 1 << ". " << movies[i]->nama << " | " << fixed << setprecision(1) << movies[i]->rating << "\n";
    }

    char pilih;
    cout << "[C]lose [R]ate\n";
    cin >> pilih;

    if (pilih == 'R' || pilih == 'r') {
        rateMovieFromList(movies);
    }
}

void genreMenu() {
    cout << "==== Pilih Genre ====\n";

    for (int i = 0; i < genreCount; i++) {
        cout << i + 1 << ". " << genreList[i] << endl;
    }

    int pilih;

    cout << "Pilih: ";
    cin >> pilih;

    if (pilih < 1 || pilih > genreCount) {
        cout << "Pilihan tidak valid\n";
        return;
    }

    string genreName = genreList[pilih - 1];

    Genre* genre = findGenre(genreName);

    if (!genre) return;

    GenreMovie* temp = genre->movieHead;

    while (temp) {
        cout << endl;
        displayMovie(temp->movie);
        temp = temp->next;
    }
}

void tambahFilm() {
    string nama;
    string studio;

    int episode;
    int season;

    cin.ignore();

    cout << "Nama: ";
    getline(cin, nama);

    if (searchMovie(root, nama)) {
        cout << nama << " sudah terdaftar di database\n";
        return;
    }

    cout << "Jumlah Episode: ";
    cin >> episode;

    cout << "Jumlah Season: ";
    cin >> season;

    cin.ignore();

    cout << "Studio: ";
    getline(cin, studio);

    Movie* newMovie = createMovie(
        nama,
        episode,
        season,
        studio
    );

    root = insertMovie(root, newMovie);

    int totalGenre;

    cout << "Jumlah genre: ";
    cin >> totalGenre;

    cin.ignore();

    for (int i = 0; i < totalGenre; i++) {
        string genre;

        cout << "Genre " << i + 1 << ": ";
        getline(cin, genre);

        connectGenre(newMovie, genre);
    }

    int totalRelated;

    cout << "Jumlah film terkait: ";
    cin >> totalRelated;

    cin.ignore();

    for (int i = 0; i < totalRelated; i++) {
        string related;

        cout << "Film terkait " << i + 1 << ": ";
        getline(cin, related);

        connectRelatedMovie(newMovie, related);
    }

    cout << "Film berhasil ditambahkan\n";

    saveToFile();
}

void saveMovie(Movie* movie, ofstream& file) {
    if (!movie) return;

    saveMovie(movie->left, file);

    file << movie->nama << "|";
    file << movie->episode << "|";
    file << movie->season << "|";
    file << movie->studio << "|";
    file << movie->rating << "|";
    file << movie->totalRating << "|";

    GenreList* genreTemp = movie->genreHead;

    while (genreTemp) {
        file << genreTemp->nama;

        if (genreTemp->next) {
            file << ",";
        }

        genreTemp = genreTemp->next;
    }

    file << "|";

    RelatedMovie* relatedTemp = movie->relatedHead;

    while (relatedTemp) {
        file << relatedTemp->nama;

        if (relatedTemp->next) {
            file << ",";
        }

        relatedTemp = relatedTemp->next;
    }

    file << endl;

    saveMovie(movie->right, file);
}

void saveToFile() {
    ofstream file("movie.txt");

    saveMovie(root, file);

    file.close();
}

void splitGenre(Movie* movie, string data) {
    string temp = "";

    for (int i = 0; i <= data.length(); i++) {
        if (data[i] == ',' || i == data.length()) {
            string genre = trimString(temp);
            if (genre != "") {
                connectGenre(movie, genre);
            }

            temp = "";
        }

        else {
            temp += data[i];
        }
    }
}

void splitRelated(Movie* movie, string data) {
    string temp = "";

    for (int i = 0; i <= data.length(); i++) {
        if (data[i] == ',' || i == data.length()) {
            string related = trimString(temp);
            if (related != "") {
                connectRelatedMovie(movie, related);
            }

            temp = "";
        }

        else {
            temp += data[i];
        }
    }
}

struct MovieLoadInfo {
    Movie* movie;
    string related;
};

void loadFromFile() {
    ifstream file("movie.txt");

    if (!file.is_open()) return;

    vector<MovieLoadInfo> loadedMovies;

    while (!file.eof()) {
        string nama;
        getline(file, nama, '|');

        if (nama == "") break;

        string episode;
        string season;
        string studio;
        string rating;
        string totalRating;
        string genre;
        string related;

        getline(file, episode, '|');
        getline(file, season, '|');
        getline(file, studio, '|');
        getline(file, rating, '|');
        getline(file, totalRating, '|');
        getline(file, genre, '|');
        getline(file, related);

        Movie* newMovie = createMovie(
            nama,
            stoi(episode),
            stoi(season),
            studio
        );

        newMovie->rating = stof(rating);
        newMovie->totalRating = stoi(totalRating);

        root = insertMovie(root, newMovie);

        splitGenre(newMovie, genre);

        loadedMovies.push_back({ newMovie, related });
    }

    file.close();

    for (auto& item : loadedMovies) {
        splitRelated(item.movie, item.related);
    }
}

void dummyData() {
    Movie* a = createMovie(
        "Interstellar",
        0,
        0,
        "Warner Bros"
    );

    Movie* b = createMovie(
        "Deadpool",
        0,
        0,
        "Marvel"
    );

    Movie* c = createMovie(
        "Breaking Bad",
        62,
        5,
        "AMC"
    );

    root = insertMovie(root, a);
    root = insertMovie(root, b);
    root = insertMovie(root, c);

    connectGenre(a, "Drama");
    connectGenre(a, "Action");

    connectGenre(b, "Action");
    connectGenre(b, "Drama");

    connectGenre(c, "Drama");

    connectRelatedMovie(b, "Interstellar");
}

int main() {
    initGenre();
    loadFromFile();

    if (!root) {
        dummyData();
        saveToFile();
    }

    bool admin = false;

    int pilih;

    do {
        cout << "\n==== Wordboxd ====\n";
        cout << "1. Genre\n";
        cout << "2. Search Film / Series\n";
        cout << "3. Semua Film\n";
        cout << "4. Semua Series\n";

        if (admin) {
            cout << "5. Tambah Film / Series\n";
        }

        cout << "Pilihan: ";
        cin >> pilih;

        if (pilih == 0) {
            string pass;

            cout << "Passphrase: ";
            cin >> pass;

            if (pass == "akuadmin727") {
                admin = true;
                cout << "Login admin berhasil\n";
            }

            else {
                cout << "Passphrase salah\n";
            }
        }

        switch (pilih) {
            case 1:
                genreMenu();
                break;

            case 2:
                searchMenu();
                break;

            case 3:
                allFilmMenu();
                break;

            case 4:
                allSeriesMenu();
                break;

            case 5:
                if (admin) {
                    tambahFilm();
                }
                break;
        }

    } while (pilih != 99);

    return 0;
}
