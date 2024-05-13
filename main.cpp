#include <iostream>
#include <fstream>
#include "Base.h"
#include "Post.h"
#include "SocialMedia.h"
#include "Date.h"
#include "SocialMedia.cpp"
#include "Post.cpp"
#include "Base.cpp"
#include "Date.cpp"
#include <windows.h>

using namespace std;

class Page;
class Post;
class Base;
class Date;
class User;
class Activity;
class Memory;
class Comment;
class SocialMediaApp;

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTitle(TEXT("Social Media Dashboard"));
    // text color: orange
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);

    SocialMediaApp *app = SocialMediaApp::getapp();
    app->ReadDataFromFile();
    app->run(app);
    delete app;
}
