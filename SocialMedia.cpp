#include <iostream>
#include "SocialMedia.h"
#include "Post.h"
#include "Base.h"
#include "Date.h"
#include <fstream>
#include <string>
#include <cstring>

class Post;
class Page;
class User;
class Base;
class Date;
using namespace std;
SocialMediaApp *SocialMediaApp::app = nullptr;

SocialMediaApp::SocialMediaApp() : currentuser(nullptr), users(nullptr), pages(nullptr), posts(nullptr), nousers(0), nopages(0), noposts(0), totalcomments(0)
{ // constructor
    userfile = "User.txt";
    pagefile = "Page.txt";
    postfile = "Post.txt";
    commentfile = "Comment.txt";
}

SocialMediaApp::~SocialMediaApp()
{ // destructor
    if (users)
    {
        for (int i = 0; i < nousers; i++)
            if (users[i])

                delete[] users;
        users = nullptr;
    }
    if (pages)
    {
        for (int i = 0; i < nopages; i++)
            if (pages[i])
                delete[] pages;
        pages = nullptr;
    }
    if (posts)
    {
        for (int i = 0; i < noposts; i++)
            if (posts[i])
                delete posts[i];
        delete[] posts;
        posts = nullptr;
    }
}

void SocialMediaApp::ReadDataFromFile()
{ // loading various types of data  users, pages, posts,comment.

    ifstream myfile;
    myfile.open(userfile);
    if (!myfile.is_open())
    {
        cout << "File do not exist." << endl;
        return;
    }

    myfile >> nousers; // 20
    users = new User *[nousers];
    // memory allocation
    // num users and there data(user,friends,id,lenght)
    char ***userFriends = new char **[nousers]; // nouser
    int *noOfFriends = new int[nousers];
    char ***userLikedPages = new char **[nousers]; // nouser
    int *noOfLikedPages = new int[nousers];
    for (int i = 0; i < nousers; i++)
    {
        userFriends[i] = new char *[maxfriends]; // maxfriends
        for (int j = 0; j < maxfriends; j++)
            userFriends[i][j] = new char[maxidlength]; // u7

        userLikedPages[i] = new char *[maxlikedpages]; // maxpages
        for (int j = 0; j < maxlikedpages; j++)
            userLikedPages[i][j] = new char[maxidlength]; // p1
    }

    LoadUsers(myfile, userFriends, noOfFriends, userLikedPages, noOfLikedPages); // loading users
    myfile.close();
    myfile.open(pagefile);
    if (myfile.is_open())
    {
        myfile >> nopages;           // 12
        pages = new Page *[nopages]; // memory allocation
        LoadPages(myfile);           // loading pages
        myfile.close();
    }
    else
    {
        cout << "File not found.";
    }
    setfriends(userFriends, noOfFriends);          // adding friends
    setlikedpages(userLikedPages, noOfLikedPages); // adding pages

    myfile.open(postfile);
    if (myfile.is_open())
    {
        myfile >> noposts;           // 12
        posts = new Post *[noposts]; // memory allocation
        LoadPosts(myfile);           // loading posts
        myfile.close();
    }
    else
    {
        cout << "File not found.";
    }
    myfile.open(commentfile);
    if (myfile.is_open())
    {
        myfile >> totalcomments; // 12
        LoadComments(myfile);    // loading comments
    }
    else
    {
        cout << "File not found.";
    }

    // deallocation
    for (int i = 0; i < nousers; i++)
    {
        if (userFriends[i] && userFriends)
        {
            for (int j = 0; j < maxfriends; j++)
                if (userFriends[i][j])
                    delete[] userFriends[i][j];
            delete[] userFriends[i];
        }
        if (userLikedPages[i] && userLikedPages)
        {
            for (int j = 0; j < maxlikedpages; j++)
                if (userLikedPages[i][j])
                    delete[] userLikedPages[i][j];
            delete[] userLikedPages[i];
        }
    }

    if (userFriends)
        delete[] userFriends;
    delete[] noOfFriends;
    userFriends = nullptr;
    noOfFriends = nullptr;

    if (userLikedPages)
        delete[] userLikedPages;
    delete[] noOfLikedPages;
    userLikedPages = nullptr;
    noOfLikedPages = nullptr;
}

void SocialMediaApp::LoadUsers(ifstream &myfile, char ***userFriends, int *friendsCount, char ***userLikedPages, int *numlikedpages)
{
    for (int i = 0; i < nousers; i++)
    {
        users[i] = new User(myfile, userFriends[i], friendsCount[i], userLikedPages[i], numlikedpages[i]);
    }
}

void SocialMediaApp::LoadPages(ifstream &myfile)
{
    for (int i = 0; i < nopages; i++)
        pages[i] = new Page(myfile);
}

void SocialMediaApp::LoadPosts(ifstream &myfile)
{
    myfile.ignore();
    myfile.ignore(100, '\n');

    // memory allocation
    char **likersList = new char *[nousers];
    for (int i = 0; i < nousers; i++)
        likersList[i] = new char[maxidlength];

    for (int i = 0; i < noposts; i++)
    {
        int nooflikes = 0;
        char currentuserId[maxidlength];

        posts[i] = new Post(myfile, (char *)currentuserId, likersList, nooflikes); // storing posts
        // to char*
        Base *currentuser;
        if (currentuserId[0] == 'u')
        {
            currentuser = (Base *)getuserbyID(currentuserId); // find user
        }
        else
        {
            currentuser = (Base *)getpagebyID(currentuserId); // fing page
        }

        if (currentuser) // check
        {
            currentuser->AddPost(posts[i]); // add post
            posts[i]->SetUser(currentuser); // add user

            for (int j = 0; j < nooflikes; j++)
            {
                Base *liker;
                if (likersList[j][0] == 'u')
                {
                    liker = (Base *)getuserbyID(likersList[j]); // find user
                }
                else
                {
                    liker = (Base *)getpagebyID(likersList[j]); // find page
                }
                if (liker)
                {
                    posts[i]->AddLiker(liker);
                }
            }
        }

        myfile.ignore();
        myfile.ignore(100, '\n'); // skip line
    }
    // deallocation
    if (likersList)
    {
        for (int i = 0; i < nousers; i++)
            if (likersList[i])
                delete[] likersList[i];

        delete[] likersList;
    }
}

void SocialMediaApp::LoadComments(ifstream &myfile)
{ // memory allocation
    for (int i = 0; i < totalcomments; i++)
    {
        char theuserID[10], postID[10];
        Post *post = nullptr;
        Comment *temp = new Comment(myfile, postID, theuserID);
        if (strlen(theuserID) > 0)
        {
            Base *theuser = nullptr;
            if (theuserID[0] == 'p') // check
            {
                theuser = (Base *)getpagebyID(theuserID); // find page
            }
            else
            {
                theuser = getuserbyID(theuserID); // or using users
            }

            post = getpostbyID(postID);
            if (post)
            {
                post->AddComment(temp);
                if (theuser)
                    temp->setpostuser(theuser); // who posted
                break;
            }
            else
            {
                delete temp;
                temp = nullptr;
            }
        }
    }
}

void SocialMediaApp::setfriends(char ***userFriends, int *friendsCount)
{
    for (int i = 0; i < nousers; i++)
    {
        for (int j = 0; j < friendsCount[i]; j++)
        {
            User *newFriend = getuserbyID(userFriends[i][j]); // getting users
            users[i]->AddFriend(newFriend);                   // adding friends
        }
    }
}

void SocialMediaApp::setlikedpages(char ***userLikedPages, int *numlikedpages)
{
    for (int i = 0; i < nousers; i++)
    {
        for (int j = 0; j < numlikedpages[i]; j++)
        {
            Page *likedPage = getpagebyID(userLikedPages[i][j]); // find liked pages

            users[i]->LikePage(likedPage); // add page
            likedPage->AddLiker(users[i]); // add liker
        }
    }
}

User *SocialMediaApp::getuserbyID(const char *userID)
{
    if (userID && userID[0] == 'u') // check
    {
        for (int i = 0; i < nousers; i++)
        {
            if (users[i] && strcmp(users[i]->getuserID(), userID) == 0) // checking both user equal(string)
            {
                return users[i];
            }
        }
    }
    else
    {
        return nullptr;
    }
}

Page *SocialMediaApp::getpagebyID(const char *pageID)
{
    if (pageID && pageID[0] == 'p') // check
    {
        for (int i = 0; i < nopages; i++)
        {
            if (pages[i] && strcmp(pages[i]->getuserID(), pageID) == 0) // checking both pages equal(string)
            {
                return pages[i];
            }
        }
    }
    else
    {
        return nullptr;
    }
}

Post *SocialMediaApp::getpostbyID(const char *postID)
{
    const char *temp = "post";

    if (strncmp(postID, temp, strlen(temp)) == 0) // checking if starts with "post"
    {
        for (int i = 0; i < noposts; i++)
        {
            if (posts[i] && strcmp(posts[i]->getID(), postID) == 0) // checking post(string) equal
            {
                return posts[i];
            }
        }
    }
    else
    {
        return nullptr;
    }
}

void SocialMediaApp::setcurrentuser(const char *userID)
{
    const char *temp = "u";
    if (strncmp(userID, temp, strlen(temp)) == 0) // checking if equal
    {
        currentuser = getuserbyID(userID); // find user
        currentuser->PrintName();
        cout << " set as Current User." << endl;
    }
}

void SocialMediaApp::ViewHome()
{

    currentuser->ViewHome(); // user class homepage
}

void SocialMediaApp::ViewTimeline()
{

    currentuser->ViewTimeline(); // user class viewtimeline
}

void SocialMediaApp::Viewpostlikedlist(const char *postID)
{
    Post *post = getpostbyID(postID); // storing post
    if (!post)
    {
        cout << "Post not found!" << endl;
        return;
    }

    post->PrintLikedList(); // post class
}

bool SocialMediaApp::likepost(const char *postID)
{

    Post *post = getpostbyID(postID); // storing post
    if (post)
    {
        return post->AddLiker(currentuser); // add who added like
    }
    else
    {
        return false;
    }
}

bool SocialMediaApp::PostComment(const char *postID, const char *text)
{

    Post *post = getpostbyID(postID); // store post

    if (post)
    {
        Comment *newComment = new Comment(('c' + to_string(totalcomments++)).c_str(), text, currentuser);
        bool status = post->AddComment(newComment); // add comment

        if (status)
            return true;

        delete newComment;
        return false;
    }

    return false;
}

void SocialMediaApp::ViewPost(const char *postID)
{
    Post *post = getpostbyID(postID); // store post

    if (post)
        post->Print(true); // print
}

void SocialMediaApp::ViewfriendLlist()
{

    currentuser->PrintFriendList(); // print
}

void SocialMediaApp::Viewlikedpageslist()
{

    currentuser->Printlikedpageslist(); // print
}

void SocialMediaApp::ViewPage(const char *pageID)
{
    Page *page = getpagebyID(pageID); // storing pages
    if (page)
        page->ViewTimeline(); // print
}

void SocialMediaApp::PrintMemories()
{ // print

    cout << endl
         << "We hope you enjoy looking back and sharing your memories on our app,";
    cout << "from the most recent to those long ago." << '\n'
         << endl;

    currentuser->PrintMemories();
}

bool SocialMediaApp::ShareMemory(const char *postID, const char *line)
{

    Post *post = getpostbyID(postID); // stores post

    if (post && post->getuser() == currentuser) // check
    {
        Memory *newPost = new Memory(("post" + to_string(noposts + 1)).c_str(), line, Date::gettodaysdate(), currentuser, post);
        bool status = currentuser->AddPost(newPost);

        if (!status)
        {
            delete newPost;
            return false;
        }
        // memory allocation
        Post **newPostlist = new Post *[noposts + 1];
        for (int i = 0; i < noposts; i++)
            newPostlist[i] = this->posts[i];
        newPostlist[noposts++] = newPost;

        delete[] this->posts;
        this->posts = newPostlist; // adding new address
        return true;
    }

    return false;
}

void SocialMediaApp::run(SocialMediaApp *app)
{
    const int noOfCommands = 12;
    string validCommands[] = {
        "  |View Home",
        "  |View Timeline",
        "  |View Post",
        "  |View Friend List",
        "  |View Liked Pages",
        "  |View Liked List",
        "  |View Memories",
        "  |View Page",
        "  |Like Post",
        " |Post Comment",
        " |Share Memory",
        " |Exit"};

    int commandIndex = 0;
    string inputID, line;
    Date tempDate;
    bool exit = false;

    cout << "\n==========================================\n";
    cout << " Welcome to the Social Media Application! \n";
    cout << "==========================================\n";

    cout << "\n|Enter user ID to set as current user (e.g., u7): ";
    getline(cin, inputID); // u7
    cout << endl;
    app->setcurrentuser(inputID.c_str()); // set current user
    cout << endl;
    cout << "Enter current system date (dd mm yyyy): " << endl;
    int day, month, year;
    cout << "|Enter Date: ";
    cin >> day;
    while (day > 31 || day < 0) // check for day
    {
        cout << "|Enter Date: ";
        cin >> day;
    }

    cout << "|Enter Month: ";
    cin >> month;
    while (month > 12 || month < 0) // check for month
    {
        cout << "|Enter Month: ";
        cin >> month;
    }
    cout << "|Enter Year: ";
    cin >> year;
    while (year > 2025 || year < 1999) // check for year
    {
        cout << "|Enter Year: ";
        cin >> year;
    }
    cin.ignore();
    tempDate = Date(day, month, year);
    Date::settodaysdate(tempDate); // setting date
    cout << endl;
    cout << "System Date set to: ";
    tempDate.Print();
    cout << endl;

    cout << "\n==========================================\n";
    cout << "Please select an option from the menu:\n";
    for (int i = 0; i < noOfCommands; i++)
    {
        cout << "  " << i + 1 << ". " << validCommands[i] << '\n';
    }
    cout << "==========================================\n";

    while (!exit) // for the loop
    {
        cout << "\n==========================================\n";
        cout << "Enter your choice (1-" << noOfCommands << "): ";
        cin >> commandIndex;
        while (commandIndex < 0 || commandIndex > 12) // check
        {
            cout << "Enter again:" << endl;
            cin >> commandIndex;
        }
        cin.ignore();
        commandIndex -= 1;

        switch (commandIndex)
        {
        case 0: // view homw
            app->ViewHome();
            break;
        case 1: // view timeline
            app->ViewTimeline();
            break;
        case 2: // view post
            cout << "Enter post ID to view (e.g., post8): ";
            getline(cin, inputID);
            app->ViewPost(inputID.c_str());
            break;
        case 3: // view friend list
            app->ViewfriendLlist();
            break;
        case 4: // view liked pages
            app->Viewlikedpageslist();
            break;
        case 5: // view post liked list
            cout << "Enter post ID to view its liked list (e.g., post8): ";
            getline(cin, inputID);
            app->Viewpostlikedlist(inputID.c_str());
            break;
        case 6: // print memories
            app->PrintMemories();
            break;
        case 7: // view page
            cout << "Enter page ID to view (e.g., p2): ";
            getline(cin, inputID);
            app->ViewPage(inputID.c_str());
            break;
        case 8: // like a post
            cout << "Enter post ID to like (e.g., post8): ";
            getline(cin, inputID);
            app->likepost(inputID.c_str());
            cout << "Post has been Liked." << endl;

            break;
        case 9: // post a comment
            cout << "Enter post ID to comment on (e.g., p2): ";
            getline(cin, inputID);
            cout << "Enter your comment: ";
            getline(cin, line);
            app->PostComment(inputID.c_str(), line.c_str());
            cout << "Comment has been Posted." << endl;

            break;
        case 10: // share memory
            cout << "Enter post ID to share as a memory (e.g., post8): ";
            getline(cin, inputID);
            cout << "Enter memory content: ";
            getline(cin, line);
            app->ShareMemory(inputID.c_str(), line.c_str());
            cout << "Memory has been Shared." << endl;
            break;
        case 11: // exist
            cout << "Exiting program." << endl;
            exit = true;
            break;
        default:
            cout << "Error." << endl;
            break;
        }
    }
}