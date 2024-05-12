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

SocialMediaApp::SocialMediaApp()
{
    currentuser = nullptr;

    userfile = "User.txt";
    pagefile = "Page.txt";
    postfile = "Post.txt";
    commentfile = "Comment.txt";

    users = nullptr;
    pages = nullptr;
    posts = nullptr;

    nousers = 0;
    nopages = 0;
    noposts = 0;
    totalcomments = 0;
}

SocialMediaApp::~SocialMediaApp()
{
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
{
    ifstream myfile;

    myfile.open(userfile);

    if (!myfile.is_open())
        return;

    myfile >> nousers;

    users = new User *[nousers];
    char ***userFriends = new char **[nousers];
    int *noOfFriends = new int[nousers];
    char ***userLikedPages = new char **[nousers];
    int *noOfLikedPages = new int[nousers];

    for (int i = 0; i < nousers; i++)
    {
        userFriends[i] = new char *[maxfriends];
        for (int j = 0; j < maxfriends; j++)
            userFriends[i][j] = new char[maxidlength];

        userLikedPages[i] = new char *[maxlikedpages];
        for (int j = 0; j < maxlikedpages; j++)
            userLikedPages[i][j] = new char[maxidlength];
    }

    LoadUsers(myfile, userFriends, noOfFriends, userLikedPages, noOfLikedPages);

    myfile.close();

    myfile.open(pagefile);

    if (myfile.is_open())
    {
        myfile >> nopages;

        pages = new Page *[nopages];

        LoadPages(myfile);

        myfile.close();
    }
    SetFriends(userFriends, noOfFriends);
    SetLikedPages(userLikedPages, noOfLikedPages);

    bool deleteUserFriends = false, deleteUserLikedPages = false;

    if (userFriends)
        deleteUserFriends = true;

    if (userLikedPages)
        deleteUserLikedPages = true;

    for (int i = 0; i < nousers; i++)
    {
        if (userFriends[i] && deleteUserFriends)
        {
            for (int j = 0; j < maxfriends; j++)
                if (userFriends[i][j])
                    delete[] userFriends[i][j];

            delete[] userFriends[i];
        }
        if (userLikedPages[i] && deleteUserLikedPages)
        {
            for (int j = 0; j < maxlikedpages; j++)
                if (userLikedPages[i][j])
                    delete[] userLikedPages[i][j];

            delete[] userLikedPages[i];
        }
    }

    if (deleteUserFriends)
        delete[] userFriends;
    delete[] noOfFriends;
    userFriends = nullptr;
    noOfFriends = nullptr;

    if (deleteUserLikedPages)
        delete[] userLikedPages;
    delete[] noOfLikedPages;
    userLikedPages = nullptr;
    noOfLikedPages = nullptr;

    myfile.open(postfile);

    if (myfile.is_open())
    {
        myfile >> noposts;

        posts = new Post *[noposts];

        LoadPosts(myfile);

        myfile.close();
    }

    myfile.open(commentfile);

    if (myfile.is_open())
    {
        myfile >> totalcomments;

        LoadComments(myfile);
    }
}

void SocialMediaApp::LoadUsers(ifstream &myfile, char ***userFriends, int *friendsCount, char ***userLikedPages, int *likedPagesCount)
{
    for (int i = 0; i < nousers; i++)
    {
        users[i] = new User(myfile, userFriends[i], friendsCount[i], userLikedPages[i], likedPagesCount[i]);
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

    char **likersList = new char *[nousers];
    for (int i = 0; i < nousers; i++)
        likersList[i] = new char[maxidlength];

    for (int i = 0; i < noposts; i++)
    {
        int noOfLikes = 0;
        char ownerId[maxidlength];

        posts[i] = new Post(myfile, (char *)ownerId, likersList, noOfLikes);

        Base *owner = (ownerId[0] == 'u') ? (Base *)SearchUserByID(ownerId) : (Base *)SearchPageByID(ownerId);

        if (owner)
        {
            owner->AddPost(posts[i]);
            posts[i]->SetUser(owner);

            for (int j = 0; j < noOfLikes; j++)
            {
                Base *liker = (likersList[j][0] == 'u') ? (Base *)SearchUserByID(likersList[j]) : (Base *)SearchPageByID(likersList[j]);
                if (liker)
                    posts[i]->AddLiker(liker);
            }
        }

        myfile.ignore();
        myfile.ignore(100, '\n');
    }

    if (likersList)
    {
        for (int i = 0; i < nousers; i++)
            if (likersList[i])
                delete[] likersList[i];

        delete[] likersList;
    }
}

void SocialMediaApp::LoadComments(ifstream &myfile)
{
    for (int i = 0; i < totalcomments; i++)
    {
        char ownerID[10], postID[10];

        Base *owner = nullptr;
        Post *post = nullptr;
        Comment *temp = new Comment(myfile, postID, ownerID);
        if (strlen(ownerID) > 0)
        {
            owner = (ownerID[0] == 'p') ? (Base *)SearchPageByID(ownerID) : SearchUserByID(ownerID);

            post = getPostByID(postID);
            if (post)
            {
                post->AddComment(temp);
                if (owner)
                    temp->setpostuser(owner);
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

void SocialMediaApp::SetFriends(char ***userFriends, int *friendsCount)
{
    for (int i = 0; i < nousers; i++)
    {
        for (int j = 0; j < friendsCount[i]; j++)
        {
            User *newFriend = SearchUserByID(userFriends[i][j]);

            if (newFriend)
                users[i]->AddFriend(newFriend);
        }
    }
}

void SocialMediaApp::SetLikedPages(char ***userLikedPages, int *likedPagesCount)
{
    for (int i = 0; i < nousers; i++)
    {
        for (int j = 0; j < likedPagesCount[i]; j++)
        {
            Page *likedPage = SearchPageByID(userLikedPages[i][j]);

            if (likedPage)
            {
                users[i]->LikePage(likedPage);
                likedPage->AddLiker(users[i]);
            }
        }
    }
}

User *SocialMediaApp::SearchUserByID(const char *userID)
{
    if (!userID || userID[0] != 'u')
        return nullptr;

    for (int i = 0; i < nousers; i++)
    {
        if (users[i] && strcmp(users[i]->getUserID(), userID) == 0)
        {
            return users[i];
        }
    }

    return nullptr;
}

Page *SocialMediaApp::SearchPageByID(const char *pageID)
{
    if (!pageID || pageID[0] != 'p')
        return nullptr;

    for (int i = 0; i < nopages; i++)
    {
        if (pages[i] && strcmp(pages[i]->getUserID(), pageID) == 0)
        {
            return pages[i];
        }
    }

    return nullptr;
}

Post *SocialMediaApp::getPostByID(const char *postID)
{
    const char *temp = "post";

    if (strncmp(postID, temp, strlen(temp)) != 0)
        return nullptr;

    for (int i = 0; i < noposts; i++)
    {
        if (posts[i] && strcmp(posts[i]->getID(), postID) == 0)
            return posts[i];
    }

    return nullptr;
}

void SocialMediaApp::SetCurrentUser(const char *userID)
{
    const char *idPrefix = "u";

    if (strncmp(userID, idPrefix, strlen(idPrefix)) != 0)
        return;

    currentuser = SearchUserByID(userID);

    if (currentuser)
    {
        currentuser->PrintName();
        cout << " successfully set as Current User." << endl;
    }
}

void SocialMediaApp::ViewHome()
{
    if (!currentuser)
    {
        cout << "Please set the current user first" << endl;
        return;
    }

    currentuser->ViewHome();
}

void SocialMediaApp::ViewTimeline()
{
    if (!currentuser)
    {
        cout << "Please set the current user first" << endl;
        return;
    }

    currentuser->ViewTimeline();
}

void SocialMediaApp::ViewPostLikedList(const char *postID)
{
    Post *post = getPostByID(postID);

    if (!post)
    {
        cout << "Post not found!" << endl;
        return;
    }

    post->PrintLikedList();
}

bool SocialMediaApp::LikePost(const char *postID)
{
    if (!currentuser)
    {
        cout << "Please set the current user first" << endl;
        return false;
    }

    Post *post = getPostByID(postID);

    if (post)
        return post->AddLiker(currentuser);

    return false;
}

bool SocialMediaApp::PostComment(const char *postID, const char *text)
{
    if (!currentuser)
    {
        cout << "Please set the current user first" << endl;
        return false;
    }

    Post *post = getPostByID(postID);

    if (post)
    {
        Comment *newComment = new Comment(('c' + to_string(totalcomments++)).c_str(), text, currentuser);
        bool status = post->AddComment(newComment);

        if (status)
            return true;

        delete newComment;
        return false;
    }

    return false;
}

void SocialMediaApp::ViewPost(const char *postID)
{
    Post *post = getPostByID(postID);

    if (post)
        post->Print(true);
}

void SocialMediaApp::ViewFriendList()
{
    if (!currentuser)
    {
        cout << "Please set the current user first" << endl;
        return;
    }

    currentuser->PrintFriendList();
}

void SocialMediaApp::ViewLikedPagesList()
{
    if (!currentuser)
    {
        cout << "Please set the current user first" << endl;
        return;
    }

    currentuser->PrintLikedPagesList();
}

void SocialMediaApp::ViewPage(const char *pageID)
{
    Page *page = SearchPageByID(pageID);

    if (page)
        page->ViewTimeline();
}

void SocialMediaApp::PrintMemories()
{
    if (!currentuser)
    {
        cout << "Please set the current user first" << endl;
        return;
    }

    cout << endl
         << "We hope you enjoy looking back and sharing your memories on our app,";
    cout << "from the most recent to those long ago." << '\n'
         << endl;

    currentuser->PrintMemories();
}

bool SocialMediaApp::ShareMemory(const char *postID, const char *body)
{
    if (!currentuser)
    {
        cout << "Please set the current user first" << endl;
        return false;
    }

    Post *post = getPostByID(postID);

    if (post && post->getuser() == currentuser)
    {
        Memory *newPost = new Memory(("post" + to_string(noposts + 1)).c_str(), body, Date::gettodaysdate(), currentuser, post);
        bool status = currentuser->AddPost(newPost);

        if (!status)
        {
            delete newPost;
            return false;
        }

        Post **newPostlist = new Post *[noposts + 1];

        for (int i = 0; i < noposts; i++)
            newPostlist[i] = this->posts[i];

        newPostlist[noposts++] = newPost;

        delete[] this->posts;

        this->posts = newPostlist;

        return true;
    }

    return false;
}

void SocialMediaApp::run(SocialMediaApp *app)
{
    const int noOfCommands = 12;
    string validCommands[] = {
        "  |View Home  ",
        "  |Like Post",
        "  |View Liked List",
        "  |Post Comment",
        "  |View Post",
        "  |View Memories",
        "  |Share Memory",
        "  |View Timeline",
        "  |View Friend List",
        " |View Liked Pages",
        " |View Page",
        " |Exit"};

    int commandIndex = 0;
    string inputID, body;
    Date newDate;
    bool exit = false;

    cout << "\n==========================================\n";
    cout << " Welcome to the Social Media Application! \n";
    cout << "==========================================\n";

    cout << "\nEnter user ID to set as current user (e.g., u7): ";
    getline(cin, inputID);
    cout << endl;
    app->SetCurrentUser(inputID.c_str());
    cout << endl;
    cout << "Enter current system date (dd mm yyyy): " << endl;
    int day, month, year;
    cout << "|Enter Date: ";
    cin >> day;
    while (day > 31 || day < 0)
    {
        cout << "|Enter Date: ";
        cin >> day;
    }

    cout << "|Enter Month: ";
    cin >> month;
    while (month > 12 || month < 0)
    {
        cout << "|Enter Month: ";
        cin >> month;
    }
    cout << "|Enter Year: ";
    cin >> year;
    while (year > 2025 || year < 1999)
    {
        cout << "|Enter Year: ";
        cin >> year;
    }
    cin.ignore();
    newDate = Date(day, month, year);
    Date::settodaysdate(newDate);
    cout << endl;
    cout << "System Date set to: ";
    newDate.Print();
    cout << endl;

    cout << "\nPlease select an option from the menu:\n";
    for (int i = 0; i < noOfCommands; i++)
    {
        cout << "  " << i + 1 << ". " << validCommands[i] << '\n';
    }

    while (!exit)
    {
        cout << "\n------------------------------------------\n";
        cout << "Enter your choice (1-" << noOfCommands << "): ";
        cin >> commandIndex;
        cin.ignore();

        commandIndex -= 1;

        if (commandIndex < 0 || commandIndex >= noOfCommands)
        {
            cout << "Invalid command. Please try again." << endl;
            continue;
        }

        switch (commandIndex)
        {
        case 0:
            app->ViewHome();
            break;
        case 1:
            cout << "Enter post ID to like (e.g., post5): ";
            getline(cin, inputID);
            app->LikePost(inputID.c_str());
            break;
        case 2:
            cout << "Enter post ID to view its liked list: ";
            getline(cin, inputID);
            app->ViewPostLikedList(inputID.c_str());
            break;
        case 3:
            cout << "Enter post ID to comment on: ";
            getline(cin, inputID);
            cout << "Enter your comment: ";
            getline(cin, body);
            app->PostComment(inputID.c_str(), body.c_str());
            break;
        case 4:
            cout << "Enter post ID to view: ";
            getline(cin, inputID);
            app->ViewPost(inputID.c_str());
            break;
        case 5:
            app->PrintMemories();
            break;
        case 6:
            cout << "Enter post ID to share as a memory: ";
            getline(cin, inputID);
            cout << "Enter memory content: ";
            getline(cin, body);
            app->ShareMemory(inputID.c_str(), body.c_str());
            break;
        case 7:
            app->ViewTimeline();
            break;
        case 8:
            app->ViewFriendList();
            break;
        case 9:
            app->ViewLikedPagesList();
            break;
        case 10:
            cout << "Enter page ID to view: ";
            getline(cin, inputID);
            app->ViewPage(inputID.c_str());
            break;
        case 11:
            cout << "Exiting program." << endl;
            exit = true;
            break;
        default:
            cout << "Invalid command. Please try again." << endl;
            break;
        }
    }
}