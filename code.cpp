#include<iostream>
#include<string>
#include <fstream>
#include <iomanip>
using namespace std;

class notification;

class user{
    public:
    string name;
    string password;
    string email;

    user* next;
    user* prev;

    //fifo 
    //appends new friend request + posts like to queue rear
    //displays oldest alert from queue front;
    notification* queuefront;
    notification* queuerear;

    user(string n , string p , string e){
        name = n;
        password = p;
        email = e;
        next = NULL;
        prev = NULL;
        queuefront = NULL;
        queuerear = NULL;
    }
};
//notification queue node:
class notification{
    public:
    string message;
    notification* next;
};
class PostNode {
public:
    int postID;
    string content;
    string author;
    int likes;

    PostNode* next;

    PostNode(int id, string c, string a) {
        postID = id;
        content = c;
        author = a;
        likes = 0;
        next = NULL;
    }
};
class PostLinkedList {
public:

    PostNode* head;
    PostNode* tail;
    int nextID;

PostLinkedList() {
    head = NULL;
    tail = NULL;
    nextID = 1;
}
void create_post(string content, string author) {

    PostNode* new_post =
        new PostNode(nextID, content, author);

    nextID++;

    // If list is empty
    if(head == NULL) {
        head = tail = new_post;
    }

    // Otherwise append at rear
    else {
        tail->next = new_post;
        tail = new_post;
    }

    cout <<"post created successfully"<<endl;
}

void sort_by_likes() {

    if (head == NULL || head->next == NULL) {
        return;
    }

    bool swapped;

    do {
        swapped = false;
        PostNode* current = head;

        while (current->next != NULL) {

            if (current->likes < current->next->likes) {

                swap(current->postID, current->next->postID);
                swap(current->content, current->next->content);
                swap(current->author, current->next->author);
                swap(current->likes, current->next->likes);

                swapped = true;
            }

            current = current->next;
        }

    } while (swapped);

    cout << "Posts sorted by likes!";
}

bool delete_post(int id, string username) {

    if(head == NULL) {
        cout << "No posts available";
        return false;
    }

    PostNode* current = head;
    PostNode* previous = NULL;

    // Search for post
    while(current != NULL && current->postID != id) {
        previous = current;
        current = current->next;
    }

    // Post not found
    if(current == NULL) {
        cout << "Post not found";
        return false;
    }

    // Check ownership
    if(current->author != username) {
        cout << "You can only delete your own posts";
        return false;
    }

    // when deleting first post
    if(current == head) {
        head = head->next;

        if(head == NULL) {
            tail = NULL;
        }
    }

    // when deleting middle or last post
    else {
        previous->next = current->next;

        if(current == tail) {
            tail = previous;
        }
    }

    delete current;

    cout << "Post has been deleted , by: "<<username<<endl;
    return true;
}
void display_posts() {

    PostNode* temp = head;

    if(temp == NULL) {
        cout << "No posts available"<<endl;
        return;
    }

    while(temp != NULL) {

        cout << "\npost ID: " << temp->postID;
        cout << "\nauthor: " << temp->author;
        cout << "\ncontent: " << temp->content;
        cout << "\nlikes: " << temp->likes;
        cout<<endl<<endl;

        temp = temp->next;
    }
}
};

class social_network{
    public:
    user* head;
    user* tail;
    user* active_user;
    PostLinkedList posts;

    social_network(){
        head = NULL;
        tail = NULL;
        active_user = NULL;
    }

    void create_post(string content) {

    if(active_user == NULL) {
        cout << "Please login first";
        return;
    }

    posts.create_post(content, active_user->name);
}
void display_posts() {
    posts.display_posts();
}
    bool login(string n, string p){

    if(active_user != NULL){
        cout <<"Someone is already logged in"<<endl;
        return false;
    }

    user* found = search(n);

    if(found == NULL){
        cout << "User does not exist"<<endl;
        return false;
    }

    if(found->password != p){
        cout << "Incorrect password"<<endl;
        return false;
    }

    active_user = found;

    cout << "Login successful"<<endl;
    return true;
}

void logout(){

    if(active_user == NULL){
        cout << "No user is logged in"<<endl;
        return;
    }

    active_user = NULL;

    cout << "Logged out"<<endl;
}

    user* search(string target_name){
        user* current = head;
        while(current!= NULL){
            if(current->name == target_name){
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    bool register_user(string n , string p , string e){
        //prevent duplication

        if(search(n) != NULL){
            cout<<"user already exists"<<endl;
            return false;
        }

        user* new_user = new user(n , p , e);

        //append to rear
        if(head == NULL){
            head = tail = new_user;
        }
        else{
            tail->next = new_user;
            new_user->prev = tail;
            tail = new_user;
        }
        return true;
        
    
}
    //fifo - notification

    //append new alert to queue rear

    void enq_notification(user* targetUser , string alert_msg){
        if(targetUser == NULL){
            return;
        }
        notification* new_notification = new notification();
        new_notification->message = alert_msg;
        new_notification->next = NULL;

        if(targetUser ->queuefront == NULL){
            targetUser->queuefront = targetUser->queuerear = new_notification;
        }
        else{
            targetUser->queuerear->next = new_notification;
            targetUser->queuerear = new_notification;
        }
    }

    void dequeue_notif(user* active_user){
        if(active_user == NULL || active_user->queuefront == NULL){
            cout<<"no unread alerts";
            return;
        }

        notification* temp = active_user->queuefront;
        cout<<"new alert: "<<temp->message<<endl;

        active_user->queuefront = active_user->queuefront->next;

        if(active_user->queuefront == NULL){
            active_user->queuerear = NULL;
        }
        delete temp;
    }

    void check_inbox(user* active_user){
    if(active_user == NULL || active_user->queuefront == NULL){
        cout << "No unread alerts" << endl;
        return;
    }

    notification* temp = active_user->queuefront;

    while(temp != NULL){
        cout << temp->message << endl;
        temp = temp->next;
    }

}
void delete_post(int id) {
    if (active_user == NULL) {
        cout << "Please login first";
        return;
    }

    posts.delete_post(id, active_user->name);
}   

void like_post(int id) {

    if (active_user == NULL) {
        cout << "Please login first";
        return;
    }

    PostNode* temp = posts.head;

    while (temp != NULL && temp->postID != id) {
        temp = temp->next;
    }

    if (temp == NULL) {
        cout << "Post not found";
        return;
    }

    if (temp->author == active_user->name) {
        cout << "You cannot like your own post";
        return;
    }

    temp->likes++;

    user* author = search(temp->author);

    if (author != NULL) {
        string msg = active_user->name +
                     " liked your post: " + temp->content;

        enq_notification(author, msg);
    }

    cout << "Post liked";
}
void sort_posts() {
    posts.sort_by_likes();
}
void choose_post_to_like() {

    if (active_user == NULL) {
        cout << "Please login first" << endl;
        return;
    }

    if (posts.head == NULL) {
        cout << "No posts available" << endl;
        return;
    }

    // Display all posts
    cout << "\n===== AVAILABLE POSTS =====\n";
    posts.display_posts();

    // Ask user to choose
    int id;
    cout << "\nEnter Post ID you want to like: ";
    cin >> id;

    // Like selected post
    like_post(id);
}

//file handling:
void save_data() {

    ofstream userFile("users.txt");
    ofstream postFile("posts.txt");
    ofstream notifFile("notifications.txt");

    if (!userFile || !postFile || !notifFile) {
        cout << "Error opening files";
        return;
    }

    // Save users
    user* u = head;

    while (u != NULL) {

        userFile << quoted(u->name) << " "
                 << quoted(u->password) << " "
                 << quoted(u->email) << endl;

        // Save this user's pending notifications
        notification* n = u->queuefront;

        while (n != NULL) {
            notifFile << quoted(u->name) << " "
                      << quoted(n->message) << endl;

            n = n->next;
        }

        u = u->next;
    }

    // Save posts
    PostNode* p = posts.head;

    while (p != NULL) {

        postFile << p->postID << " "
                 << quoted(p->content) << " "
                 << quoted(p->author) << " "
                 << p->likes << endl;

        p = p->next;
    }

    cout << "Data saved successfully!";
}
void load_data() {

    ifstream userFile("users.txt");
    ifstream postFile("posts.txt");
    ifstream notifFile("notifications.txt");

    string name, password, email;

    // Load users
    while (userFile >> quoted(name)
                    >> quoted(password)
                    >> quoted(email)) {

        user* new_user = new user(name, password, email);

        if (head == NULL) {
            head = tail = new_user;
        }
        else {
            tail->next = new_user;
            new_user->prev = tail;
            tail = new_user;
        }
    }

    
    // Load posts
    int id, likes;
    string content, author;

    while (postFile >> id
                    >> quoted(content)
                    >> quoted(author)
                    >> likes) {

        PostNode* new_post = new PostNode(id, content, author);
        new_post->likes = likes;

        if (posts.head == NULL) {
            posts.head = posts.tail = new_post;
        }
        else {
            posts.tail->next = new_post;
            posts.tail = new_post;
        }

        if (id >= posts.nextID) {
            posts.nextID = id + 1;
        }
    }

    // Load pending notifications
    string username, message;

    while (notifFile >> quoted(username)
                     >> quoted(message)) {

        user* target = search(username);

        if (target != NULL) {
            enq_notification(target, message);
        }
    }

    cout << "Data loaded successfully!";
}
};



int main() {

    social_network app;

    // Register users
    app.register_user("Ali", "123", "ali@gmail.com");
    app.register_user("Sara", "456", "sara@gmail.com");

    // Sara creates posts
    app.login("Sara", "456");

    app.create_post("Learning DSA");
    app.create_post("My first project");

    app.logout();

    // Ali likes Sara's post
    app.login("Ali", "123");

    app.like_post(1);

    app.logout();

    // Sort posts by likes
    app.sort_posts();

    // Save everything
    app.save_data();

    return 0;
}
