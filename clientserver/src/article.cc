#include "article.h"

using std::string;

Article::Article(string t, string a, string tex, long long int id) : title{t}, author{a}, text{tex}, id{id} {}

Article::Article(const Article& art) : title{art.title}, author{art.author}, text{art.text}, id{art.id} {}

string Article::getTitle() {
    return title;
}

string Article::getAuthor() {
    return author;
}

string Article::getText() {
    return text;
}

long long int Article::getId() {
    return id;
}
