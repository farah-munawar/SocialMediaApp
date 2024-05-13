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

Post::Post(const char *id, const char *text, const Date &d, Base *postuser, int typeactivity, char *subtypeactivity)
    : shareDate(d), id(nullptr), text(nullptr), likers(nullptr), noOfLikers(0), comments(nullptr), noOfComments(0), theuser(postuser), activity(nullptr)
{ // memory

    if (id)
    { // memory allocation
        this->id = new char[strlen(id) + 1];
        strcpy(this->id, id);
    }
    if (text)
    {
        this->text = new char[strlen(text) + 1];
        strcpy(this->text, text);
    }
}

Post::Post(ifstream &myfile, char *theuser, char **likersList, int &numlikes)
    : likers(nullptr), noOfLikers(0), comments(nullptr), noOfComments(0), theuser(nullptr)
{ // constructor
    LoadData(myfile, theuser, likersList, numlikes);
}

void Post::LoadData(ifstream &myfile, char *theuser, char **likersList, int &numlikes)
{ // taking data from file

    char temp[100];
    int size = 0, type = 0;
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

    myfile >> theuser;

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
{ // print
    cout << "--- ";
    theuser->PrintName();

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
    // check
    if (!user)
        return false;

    // memory allocation
    if (!likers)
    {
        likers = new Base *[maxLikers];
        for (int i = 0; i < maxLikers; i++)
            likers[i] = nullptr;
    }
    // check if the user is already in the likers array

    for (int i = 0; i < noOfLikers; i++)
    {
        if (likers[i] == user) // user already liked the post
        {
            return false;
        }
    }

    // adding the user to the likers array
    if (noOfLikers < maxLikers)
    {
        likers[noOfLikers++] = user;
        return true;
    }

    return false;
}

bool Post::AddComment(Comment *com)
{
    if (!com)
    {
        return false;
    }

    if (!comments)
    { // memory allocation
        comments = new Comment *[maxComments];
        for (int i = 0; i < maxComments; i++)
            comments[i] = nullptr;
    }

    // add comment
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

void Post::SetUser(Base *user)
{
    theuser = user;
}

Base *Post::getuser()
{
    return theuser;
}

char *Post::getID()
{
    return id;
}

void Post::PrintLikedList()
{
    cout << endl;

    cout << "Post Liked By:" << endl;
    for (int i = 0; i < noOfLikers; i++)
        likers[i]->PrintDetails();
}

Memory::Memory(const char *id, const char *text, const Date &currDate, Base *postuser, Post *original)
    : Post(id, text, currDate, postuser)
{
    originalPost = original;
}

void Memory::Print(bool printDate = true, bool com = true)
{

    cout << "~~~ ";
    theuser->PrintName();

    cout << " shared a memory ~~~ ...";

    // print date
    if (printDate)
    {
        Post::getpostdate().Print();
    }

    cout << endl;

    Post::PrintText();

    if (printDate)
    {
        cout << endl;
        cout << '\t' << '\t' << '(' << Date::gettodaysdate().getYear() - originalPost->getpostdate().getYear() << " Years Ago)" << '\n';
    }
    originalPost->Print(true, false);

    if (com)
        Post::PrintComments();
}

const char *Activity::types[notypes] = {
    "feeling",
    "thinking about",
    "making",
    "celebrating"};

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
        value = new char[size + 1];
        strcpy(value, temp);
    }
    else
        value = nullptr;
}

Activity::~Activity()
{
    if (value)
        delete[] value;
}

void Activity::Print()
{ // print
    if (typeno != -1 && value)
        cout << " is " << types[typeno] << ' ' << value << endl;
}

Comment::Comment(const char *id, const char *body, Base *theuser)
    : id(nullptr), text(nullptr), postuser(theuser)
{
    // Allocating id
    if (id && strlen(id) > 0)
    {
        this->id = new char[strlen(id) + 1];
        strcpy(this->id, id);
    }

    // Allocating text
    if (body && strlen(body) > 0)
    {
        text = new char[strlen(body) + 1];
        strcpy(text, body);
    }
}

Comment::Comment(ifstream &myfile, char *postID, char *theuser) : id(nullptr), text(nullptr)
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

    myfile >> postID;
    myfile >> theuser;
    myfile.ignore();
    myfile.getline(temp, 100, '\n');
    size = strlen(temp);

    if (size > 0)
    {
        text = new char[size + 1];
        strcpy(text, temp);
    }
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
