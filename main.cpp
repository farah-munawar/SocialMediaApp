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
    SocialMediaApp *app = SocialMediaApp::getapp();
    app->ReadDataFromFile();
    app->run(app);
}
