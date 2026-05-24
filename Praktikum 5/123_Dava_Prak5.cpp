#include <iostream>
#include <fstream>
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
void displayMovie(Movie* movie);
void splitRelated(Movie* movie, string data);

const int SIZE = 5;
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

bool movieMatchesQuery(Movie* movie, const string& query) {
    string lowerQuery = toLowerCase(query);
    string lowerName = toLowerCase(movie->nama);
    return lowerName.find(lowerQuery) != string::npos;
}

void collectMovieMatches(Movie* root, const string& query, vector<Movie*>& results) {
    if (!root) return;

    collectMovieMatches(root->left, query, results);

    if (movieMatchesQuery(root, query)) {
        results.push_back(root);
    }

    collectMovieMatches(root->right, query, results);
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

bool isGenreValid(const string& genreName) {
    return findGenre(genreName) != NULL;
}

void viewGenreMovies(Genre* genre) {
    vector<Movie*> movies;
    GenreMovie* temp = genre->movieHead;

    while (temp) {
        movies.push_back(temp->movie);
        temp = temp->next;
    }

    if (movies.empty()) {
        cout << "Tidak ada film dalam genre ini\n";
        return;
    }

    int currentIdx = 0;

    do {
        cout << "\n==== Genre " << genre->nama << " ====\n";
        displayMovie(movies[currentIdx]);

        cout << "[N]ext, [P]revious, [C]lose, [R]ate\n";
        char pilih;
        cin >> pilih;

        if (pilih == 'N' || pilih == 'n') {
            if (currentIdx < (int)movies.size() - 1) {
                currentIdx++;
            }
            else {
                cout << "Sudah di film terakhir\n";
            }
        }
        else if (pilih == 'P' || pilih == 'p') {
            if (currentIdx > 0) {
                currentIdx--;
            }
            else {
                cout << "Sudah di film pertama\n";
            }
        }
        else if (pilih == 'R' || pilih == 'r') {
            rateMovie(movies[currentIdx]);
            saveToFile();
        }
        else if (pilih == 'C' || pilih == 'c') {
            break;
        }
    } while (true);
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

void searchMenu() {
    string nama;

    cin.ignore();

    cout << "Nama: ";
    getline(cin, nama);

    vector<Movie*> matches;
    collectMovieMatches(root, nama, matches);

    if (matches.empty()) {
        cout << "Film tidak ditemukan\n";
        return;
    }

    if (matches.size() == 1) {
        Movie* movie = matches[0];
        displayMovie(movie);

        char pilih;
        cout << "[C]lose [R]ate\n";
        cin >> pilih;

        if (pilih == 'R' || pilih == 'r') {
            rateMovie(movie);
            saveToFile();
        }

        return;
    }

    cout << "Hasil pencarian:\n";
    for (int i = 0; i < (int)matches.size(); i++) {
        cout << i + 1 << ". " << matches[i]->nama << " | " << fixed << setprecision(1) << matches[i]->rating << "\n";
    }

    cout << "Pilih film (0 untuk batal): ";
    int pilih;
    cin >> pilih;

    if (pilih < 1 || pilih > (int)matches.size()) {
        cout << "Dibatalkan\n";
        return;
    }

    Movie* movie = matches[pilih - 1];
    displayMovie(movie);

    char action;
    cout << "[C]lose [R]ate\n";
    cin >> action;

    if (action == 'R' || action == 'r') {
        rateMovie(movie);
        saveToFile();
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

    viewGenreMovies(genre);
}

void tambahFilm() {
    string nama;
    string studio;
    string genreInput;
    string relatedInput;

    int episode;
    int season;

    cin.ignore();

    cout << "==== Tambah Film / Series ====\n";
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

    cout << "Genre: ";
    getline(cin, genreInput);

    cout << "Jumlah Film terkait (dipisah dengan ,): ";
    getline(cin, relatedInput);

    cout << "Studio: ";
    getline(cin, studio);

    Movie* newMovie = createMovie(
        nama,
        episode,
        season,
        studio
    );

    vector<string> genres;
    string tempGenre = "";

    for (int i = 0; i <= genreInput.length(); i++) {

        if (i == genreInput.length() || genreInput[i] == ',') {
            
            string g = trimString(tempGenre);

            if (g != "") {
                genres.push_back(g);
            }

            tempGenre = "";
        }

        else {
            tempGenre += genreInput[i];
        }
    }

    int validGenreCount = 0;
    for (const auto& g : genres) {
        if (isGenreValid(g)) {
            validGenreCount++;
        }
    }

    if (validGenreCount == 0) {
        cout << "Tidak ada genre yang valid. Film ditolak\n";
        return;
    }

    root = insertMovie(root, newMovie);

    for (const auto& g : genres) {
        connectGenre(newMovie, g);
    }

    splitRelated(newMovie, relatedInput);

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

    for (int i = 0; i < (int)data.length(); i++) {
        if (data[i] == ',') {
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

    string genre = trimString(temp);
    if (genre != "") {
        connectGenre(movie, genre);
    }
}

void splitRelated(Movie* movie, string data) {
    string temp = "";

    for (int i = 0; i < (int)data.length(); i++) {
        if (data[i] == ',') {
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

    string related = trimString(temp);
    if (related != "") {
        connectRelatedMovie(movie, related);
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

int main() {
    initGenre();
    loadFromFile();

    if (!root) {
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
