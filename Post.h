#ifndef POST_H
#define POST_H

#include <fstream>
using namespace std;
#include "Base.h"
#include "Date.h"
#include "SocialMedia.h"
class Base;
class Date;
class Page;
class Comment;
class Activity
{
    static const int notypes = 4;
    static const int nosubtypes = 3;

    static const char *types[notypes];
    int typeno;
    char *subtype;

public:
    Activity(int, const char *);
    Activity(ifstream &);
    ~Activity();
    void Print();
};

class Comment
{
    char *id;
    char *text;
    Base *postuser;

public:
    Comment(const char *, const char *, Base *);
    Comment(ifstream &, char *postID, char *owner);
    ~Comment();
    void setpostuser(Base *user);
    const Base *getuser();
    void Print();
};

class Post
{
    static const int maxComments = 10;
    static const int maxLikers = 10;
    char *id;
    char *text;

    int noOfLikers;
    Base **likers;

    Date shareDate;

    Comment **comments;
    int noOfComments;

    Activity *activity;

protected:
    void PrintComments();
    void PrintText();
    Base *theuser;

public:
    Post(const char *, const char *, int, int, int, Base *, int = -1, char * = nullptr);
    Post(const char *, const char *, const Date &, Base *, int = -1, char * = nullptr);
    Post(ifstream &, char *, char **, int &);
    void LoadData(ifstream &, char *, char **, int &);
    virtual ~Post();
    Date getpostdate();
    bool AddLiker(Base *);
    bool AddComment(Comment *);
    virtual void Print(bool = false, bool = true);
    void SetUser(Base *);
    const Base *getuser();
    const char *getID();
    void PrintLikedList();
};

class Memory : public Post
{
    Post *originalPost;

public:
    Memory(const char *id, const char *text, int d, int m, int y, Base *postuser, Post *orig);
    Memory(const char *id, const char *text, const Date &currentDate, Base *postuser, Post *orig);
    void Print(bool, bool);
};
#endif