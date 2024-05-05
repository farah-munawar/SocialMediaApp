#include "Post.h"
#include "Base.h"
#include "Date.h"
#include "SocialMedia.h"

#include <cstring>
#include <iostream>
using namespace std;

class SocialMediaApp;
class Date;
class User;
class Base;
Post::Post(const char *id, const char *text, int d, int m, int y, Base *postuser, int typeactivity, char *subtypeactivity) : shareDate(d, m, y)
{
    if (id)
    {
        this->id = new char[strlen(id) + 1];
        strcpy(this->id, id);
    }
    else
        this->id = nullptr;

    if (text)
    {
        this->text = new char[strlen(text) + 1];
        strcpy(this->text, text);
    }
    else
        this->text = nullptr;

    likers = nullptr;
    noOfLikers = 0;

    comments = nullptr;
    noOfComments = 0;

    owner = postuser;

    if (typeactivity != -1 && subtypeactivity)
        activity = new Activity(typeactivity, subtypeactivity);
    else
        activity = nullptr;
}

Post::Post(const char *id, const char *text, const Date &d, Base *postuser, int typeactivity, char *subtypeactivity) : shareDate(d)
{
    if (id)
    {
        this->id = new char[strlen(id) + 1];
        strcpy(this->id, id);
    }
    else
        this->id = nullptr;

    if (text)
    {
        this->text = new char[strlen(text) + 1];
        strcpy(this->text, text);
    }
    else
        this->text = nullptr;

    likers = nullptr;

    noOfLikers = 0;

    comments = nullptr;
    noOfComments = 0;

    owner = postuser;

    if (typeactivity != -1 && subtypeactivity)
        activity = new Activity(typeactivity, subtypeactivity);
    else
        activity = nullptr;
}

Post::Post(ifstream &myfile, char *owner, char **likersList, int &numlikes)
{
    likers = nullptr;
    noOfLikers = 0;

    comments = nullptr;
    noOfComments = 0;

    this->owner = nullptr;

    LoadData(myfile, owner, likersList, numlikes);
}

void Post::LoadData(ifstream &myfile, char *owner, char **likersList, int &numlikes)
{
    int size = 0, type = 0;
    char temp[100];

    myfile >> type;
    myfile >> temp;

    size = strlen(temp);

    if (size > 0)
    {
        id = new char[size + 1];
        strcpy(id, temp);
    }
    else
        id = nullptr;

    shareDate.LoadData(myfile);

    myfile.getline(temp, 100, '\n');

    size = strlen(temp);

    if (size > 0)
    {
        text = new char[size + 1];
        strcpy(text, temp);
    }
    else
        text = nullptr;

    if (type == 2)
        activity = new Activity(myfile);
    else
        activity = nullptr;

    myfile >> owner;

    numlikes = 0;
    while (numlikes < maxLikers)
    {
        myfile >> temp;

        if (strcmp(temp, "-1") == 0)
            break;

        strcpy(likersList[numlikes++], temp);
    }
}

Post::~Post()
{
    if (id)
        delete[] id;
    if (text)
        delete[] text;

    if (comments)
    {
        for (int i = 0; i < maxComments; i++)
        {
            if (comments[i])
                delete comments[i];
            else
                break;
        }
        delete[] comments;
    }

    if (likers)
        delete[] likers;

    if (activity)
        delete activity;
}

Date Post::getpostdate()
{
    return shareDate;
}
void Post::Print(bool printDate, bool Com)
{
    cout << "--- ";
    owner->PrintName();

    if (activity)
    {
        activity->Print();
        cout << "    ";
    }
    else
    {
        cout << " shared ";
    }

    Post::PrintText();

    if (printDate)
    {
        cout << "...";
        shareDate.Print();
    }
    cout << endl;

    if (Com)
    {
        PrintComments();
    }
}
bool Post::AddLiker(Base *user)
{
    if (!user)
        return false;

    if (!likers)
    {
        likers = new Base *[maxLikers];
        for (int i = 0; i < maxLikers; i++)
            likers[i] = nullptr;
    }

    if (noOfLikers < maxLikers)
    {
        for (int i = 0; i < noOfLikers; i++)
            if (likers[i] == user)
                return true;

        likers[noOfLikers++] = user;
        return true;
    }

    return false;
}

bool Post::AddComment(Comment *com)
{
    if (!com)
        return false;

    if (!comments)
    {
        comments = new Comment *[maxComments];
        for (int i = 0; i < maxComments; i++)
            comments[i] = nullptr;
    }

    if (noOfComments < maxComments)
    {
        comments[noOfComments++] = com;

        return true;
    }

    return false;
}

void Post::PrintText()
{
    cout << "\"" << text << "\"";
}

void Post::PrintComments()
{
    for (int i = 0; i < noOfComments; i++)
    {
        if (comments[i])
        {
            cout << '\t' << '\t' << '\t';
            comments[i]->Print();
        }
        else
            break;
    }

    cout << endl;
}

void Post::SetOwner(Base *user)
{
    owner = user;
}

const Base *Post::getOwner()
{
    return owner;
}

const char *Post::getID()
{
    return id;
}

void Post::PrintLikedList()
{
    cout << std::endl;

    cout << "Post Liked By:" << endl;
    for (int i = 0; i < noOfLikers; i++)
        likers[i]->PrintDetails();
}

Memory::Memory(const char *id, const char *text, int d, int m, int y, Base *author, Post *original)
    : Post(id, text, d, m, y, author)
{
    originalPost = original;
}

Memory::Memory(const char *id, const char *text, const Date &currDate, Base *author, Post *original)
    : Post(id, text, currDate, author)
{
    originalPost = original;
}

void Memory::Print(bool printDate = true, bool com = true)
{

    cout << "~~~ ";
    owner->PrintName();

    cout << " shared a memory ~~~ ...";

    if (printDate)
        Post::getpostdate().Print();

    cout << endl;

    Post::PrintText();

    cout << endl;
    cout << '\t' << '\t' << '(';
    cout << Date::gettodaysdate().getYear() - originalPost->getpostdate().getYear();
    cout << " Years Ago)" << '\n';

    originalPost->Print(true, false);

    if (com)
        Post::PrintComments();
}
const char *Activity::types[notypes] = {
    "feeling",
    "thinking about",
    "making",
    "celebrating"};

Activity::Activity(int type, const char *subtype) : typeno(type - 1)
{
    if (typeno < 0 || typeno >= notypes)
    {
        typeno = -1;
        return;
    }

    int size = strlen(subtype);
    if (size > 0)
    {
        this->subtype = new char[size + 1];
        strcpy(this->subtype, subtype);
    }
    else
        subtype = nullptr;
}

Activity::Activity(ifstream &myfile)
{
    char temp[100];

    myfile >> typeno;
    myfile.ignore();
    typeno -= 1;

    if (typeno < 0 || typeno >= notypes)
    {
        typeno = -1;
        return;
    }

    myfile.getline(temp, 100, '\n');

    int size = strlen(temp);
    if (size > 0)
    {
        subtype = new char[size + 1];
        strcpy(subtype, temp);
    }
    else
        subtype = nullptr;
}

Activity::~Activity()
{
    if (subtype)
        delete[] subtype;
}

void Activity::Print()
{
    if (typeno != -1 && subtype)
        cout << " is " << types[typeno] << ' ' << subtype << endl;
}

Comment::Comment(const char *id, const char *body, Base *owner)
{
    if (id)
    {
        this->id = new char[strlen(id) + 1];
        strcpy(this->id, id);
    }
    else
        this->id = nullptr;

    if (body)
    {
        text = new char[strlen(body) + 1];
        strcpy(text, body);
    }
    else
        text = nullptr;

    postuser = owner;
}

Comment::Comment(ifstream &myfile, char *postID, char *owner)
{
    char temp[100];
    int size = 0;

    myfile >> temp;

    size = strlen(temp);

    if (size > 0)
    {
        id = new char[size + 1];
        strcpy(id, temp);
    }
    else
        id = nullptr;

    myfile >> postID;

    myfile >> owner;

    myfile.ignore();
    myfile.getline(temp, 100, '\n');
    size = strlen(temp);

    if (size > 0)
    {
        text = new char[size + 1];
        strcpy(text, temp);
    }
    else
        text = nullptr;
}

Comment::~Comment()
{
    if (id)
        delete[] id;

    if (text)
        delete[] text;
}

void Comment::setpostuser(Base *user)
{
    postuser = user;
}

const Base *Comment::getuser()
{
    return postuser;
}

void Comment::Print()
{
    postuser->PrintName();
    cout << " wrote: ";
    if (text)
        cout << "\"" << text << "\"" << endl;
    ;
}
