#ifndef Base_H
#define Base_H
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include "SocialMedia.h"
#include "Post.h"
#include "Date.h"

using namespace std;
class Post; // aggregation
class Date; // dependency
class Activity;

class Base
{
    char *id;
    int noPosts;
    const int maxPosts = 10;
    Post **timeline;

public:
    Base(ifstream &);
    virtual ~Base();
    bool AddPost(Post *);
    Post *getlatestpost();
    const char *getuserID();
    void ViewTimeline();
    void PrintMemories();
    virtual void PrintName() = 0; // pure virtual class(just to connnect)
    virtual void PrintDetails();
};
class Page : public Base
{
    static const int maxLikers = 10;
    char *title;

    int nolikers;
    Base **likers;

public:
    Page(ifstream &);
    ~Page();
    bool AddLiker(Base *);
    void PrintName();
};
class User : public Base
{ // user and post association throught base class
    char *fName;
    char *lName;
    const int maxFriends = 10;
    const int maxlikedpages = 10;

    Page **likedPages; // aggregation
    int nolikedpages;

    User **friends; // aggreation
    int nofriends;

public:
    User(ifstream &, char **, int &, char **, int &);
    ~User();
    void LoadDate(ifstream &, char **, int &, char **, int &);
    bool AddFriend(User *);
    bool LikePage(Page *);
    void PrintName();
    int getfriendCount();
    int getLikedPagesCount();
    void ViewHome();
    void PrintFriendList();
    void Printlikedpageslist();
};

#endif