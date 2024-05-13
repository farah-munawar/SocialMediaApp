#include "Base.h"
#include "Post.h"
#include "Date.h"
#include "SocialMedia.h"
#include <cstring>
#include <iostream>

class Post;
class User;

// constructor
Base::Base(ifstream &myfile) : id(nullptr), timeline(nullptr), noPosts(0)
{
    char temp[10];

    myfile >> temp;
    myfile.ignore();

    int size = strlen(temp);

    if (size > 0)
    {
        id = new char[size + 1];
        strcpy(id, temp);
    }
}

Base::~Base()
{
    if (id)
    {
        delete[] id;
    }

    if (timeline)
    {
        delete[] timeline;
    }
}
// adds a new post object to the timeline array
bool Base::AddPost(Post *p)
{
    if (!timeline) // check
    {              // memory allocation
        timeline = new Post *[maxPosts];
        for (int i = 0; i < maxPosts; i++)
        {
            timeline[i] = nullptr;
        }
    }

    if (noPosts >= maxPosts) // check
    {
        return false;
    }

    int i = 0;
    // adjusting dates avaible(old to latest)
    for (i = 0; i < noPosts; i++)
    {
        if (timeline[i]->getpostdate() >= p->getpostdate())
            break;
    }

    // move posts down to make space for the new post
    for (int j = noPosts; j > i; j--)
    {
        timeline[j] = timeline[j - 1];
    }
    timeline[i] = p;
    noPosts++;
    return true;
}

// returning the lastest post
Post *Base::getlatestpost()
{
    if (noPosts <= 0)
    {
        return nullptr;
    }
    Post *temp = nullptr;
    const Date today = Date::gettodaysdate(); // stores today date

    for (int i = 0; i < noPosts; i++)
    {
        if (timeline[i]->getpostdate() <= today)
        { // if no latest has been found yet or this post is more recent then update latest

            if (temp == nullptr || temp->getpostdate() < timeline[i]->getpostdate())
                temp = timeline[i];
        }
    }

    return temp;
}

const char *Base::getUserID()
{
    return id;
}

void Base::ViewTimeline()
{
    PrintName();
    cout << " || Timeline";
    cout << endl
         << endl;

    if (noPosts == 0)
    {
        cout << "No posts to display." << endl;
    }
    else
    {
        for (int i = 0; i < noPosts; i++)
        {
            timeline[i]->Print(true, true);
        }
    }
}

void Base::PrintDetails()
{
    cout << id;
    cout << " : ";
    PrintName();
    cout << endl;
}

void Base::PrintMemories()
{
    bool check = false;
    cout << "On this Day" << endl;

    for (int i = 0; i < noPosts; i++)
    {
        // checking date and month and years<year

        if (timeline[i]->getpostdate().getDate() == Date::gettodaysdate().getDate() && timeline[i]->getpostdate().getMonth() == Date::gettodaysdate().getMonth() && timeline[i]->getpostdate().getYear() < Date::gettodaysdate().getYear())
        {
            cout << Date::gettodaysdate().getYear() - timeline[i]->getpostdate().getYear();
            cout << " Years Ago" << endl;
            timeline[i]->Print(true);
            check = true;
        }
    }

    if (!check)
    {
        cout << "No memories from this day in previous years." << endl;
    }
}

User::User(ifstream &myfile, char **friends, int &numfriends, char **likedPages, int &numlikedPages) : Base(myfile), friends(nullptr), likedPages(nullptr), nofriends(0), nolikedpages(0)
{ // constructor
    LoadData(myfile, friends, numfriends, likedPages, numlikedPages);
}

void User::LoadData(ifstream &myfile, char **friends, int &numfriends, char **likedPages, int &numlikedPages)
{ // taking data from file
    char temp[100];
    myfile >> temp;

    int size = strlen(temp);
    if (size > 0)
    {
        this->fName = new char[size + 1];
        strcpy(this->fName, temp);
    }
    else
    {
        fName = nullptr;
    }

    myfile >> temp;

    size = strlen(temp);
    if (size > 0)
    {
        this->lName = new char[size + 1];
        strcpy(this->lName, temp);
    }
    else
    {
        lName = nullptr;
    }

    // friends added
    numfriends = 0;
    while (numfriends < maxFriends)
    {
        myfile >> temp;
        if (strcmp(temp, "-1") == 0)
        {
            break;
        }
        else
        {
            strcpy(friends[numfriends++], temp);
        }
    }

    // add pages

    numlikedPages = 0;
    while (numlikedPages < maxlikedpages)
    {
        myfile >> temp;
        if (strcmp(temp, "-1") == 0)
        {
            break;
        }
        else
        {
            strcpy(likedPages[numlikedPages++], temp);
        }
    }
}

User::~User()
{
    if (fName)
    {
        delete[] fName;
    }
    if (lName)
    {
        delete[] lName;
    }

    if (likedPages)
    {
        delete[] likedPages;
    }
    if (friends)
    {
        delete[] friends;
    }
}

bool User::AddFriend(User *user)
{

    if (!friends) // check
    {             // memory allocation
        friends = new User *[maxFriends];
        for (int i = 0; i < maxFriends; i++)
        {
            friends[i] = nullptr;
        }
    }

    if (nofriends < maxFriends)
    { // adding new friends
        friends[nofriends++] = user;
        return true;
    }
    return false;
}

bool User::LikePage(Page *likedPage)
{
    if (!likedPage) // check
    {
        return false;
    }

    if (!likedPages) // check
    {                // memory allocation
        likedPages = new Page *[maxlikedpages];
        for (int i = 0; i < maxlikedpages; i++)
        {
            likedPages[i] = nullptr;
        }
    }

    if (nolikedpages < maxlikedpages)
    { // adding liked pages
        likedPages[nolikedpages++] = likedPage;
        return true;
    }
    return false;
}

void User::PrintName()
{
    if (fName)
    {
        cout << fName << ' ';
    }
    if (lName)
    {
        cout << lName;
    }
}

int User::getfriendCount()
{
    return nofriends;
}

int User::getLikedPagesCount()
{
    return nolikedpages;
}

void User::ViewHome()
{
    PrintName();

    cout << " || Home Page" << endl
         << endl;

    // display latest post from each friend
    for (int i = 0; i < nofriends; i++)
    {
        if (friends[i])
        { // Check
            Post *latestPost = friends[i]->getlatestpost();
            if (latestPost)
            {
                latestPost->Print();
            }
            else
            {
                cout << "No recent posts from friend " << i + 1 << "." << endl;
            }
        }
    }

    // display latest post from each liked page
    for (int i = 0; i < nolikedpages; i++)
    {
        if (likedPages[i])
        { // Check
            Post *latestPost = likedPages[i]->getlatestpost();
            if (latestPost)
            {
                latestPost->Print();
            }
            else
            {
                cout << "No recent posts from liked page " << i + 1 << "." << endl;
            }
        }
    }
}

void User::PrintFriendList()
{ // print friend list
    PrintName();

    cout << " || Friends" << endl;

    for (int i = 0; i < nofriends; i++)
    {
        friends[i]->PrintDetails();
    }
}

void User::PrintLikedPagesList()
{ // print liked pages only
    PrintName();

    cout << " || Liked Pages" << endl
         << endl;

    for (int i = 0; i < nolikedpages; i++)
    {
        likedPages[i]->PrintDetails();
    }
}

Page::Page(ifstream &myfile) : Base(myfile), title(nullptr), nolikers(0), likers(nullptr)
{
    char temp[100];
    if (myfile.getline(temp, 100))
    {
        int size = strlen(temp);
        if (size > 0)
        {
            title = new char[size + 1];
            strcpy(title, temp);
        }
    }
}

Page::~Page()
{
    if (title)
    {
        delete[] title;
    }

    if (likers)
    {
        delete[] likers;
    }
}

bool Page::AddLiker(Base *user)
{ // checks
    if (!user)
    {
        return false;
    }

    if (!likers)
    { // memory allocation
        likers = new Base *[maxLikers];
    }

    if (nolikers >= maxLikers) // check
    {
        return false;
    }
    // add user to the like list
    likers[nolikers++] = user;
    return true;
}

void Page::PrintName()
{
    if (title)
    {
        cout << title;
    }
}