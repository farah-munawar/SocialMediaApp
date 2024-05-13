#ifndef SOCIALMEDIA_H
#define SOCIALMEDIA_H

#include <fstream>
#include "Post.h"
#include "Base.h"
#include "Date.h"
class Post;
class User;
class Page;
class Base;
class SocialMediaApp
{
    const int maxfriends = 10;
    const int maxlikedpages = 10;
    const int maxidlength = 10;
    const char *userfile;
    const char *pagefile;
    const char *postfile;
    const char *commentfile;

    User *currentuser; // composition
    int nousers;
    int nopages;
    int noposts;
    int totalcomments;

    User **users; // aggeragation
    Page **pages; // aggeragation
    Post **posts; // ssociation

    void LoadUsers(ifstream &, char ***, int *, char ***, int *);
    void LoadPages(ifstream &);
    void LoadPosts(ifstream &);
    void LoadComments(ifstream &);

    void SetFriends(char ***, int *);
    void SetLikedPages(char ***, int *);
    User *getuserbyID(const char *);
    Page *getpagebyID(const char *);
    Post *getPostByID(const char *);

    SocialMediaApp();
    static SocialMediaApp *app;

public:
    static SocialMediaApp *getapp()
    {
        if (!app)
            app = new SocialMediaApp;

        return app;
    }

    void run(SocialMediaApp *app);
    void ReadDataFromFile();
    void ViewPostLikedList(const char *);
    void ViewFriendList();
    void ViewLikedPagesList();
    void ViewHome();
    void ViewTimeline();
    void ViewPage(const char *);
    void ViewPost(const char *);
    void PrintMemories();
    void SetCurrentUser(const char *);
    bool LikePost(const char *);
    bool PostComment(const char *, const char *);
    bool ShareMemory(const char *, const char *);
    ~SocialMediaApp();
};
#endif