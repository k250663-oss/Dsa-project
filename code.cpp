 #include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;
// forward declaration so user can point to notification nodes*
class notification;
// a registered user, stored as a node in a doubly linked list*
class user
{
public:
    string name;
    string password;
    string email;
    // links to the neighbouring users in the doubly linked list*
    user *next;
    user *prev;
    // fifo*
    // appends new friend request + posts like to queue rear*
    // displays oldest alert from queue front;*
    notification *queuefront;
    notification *queuerear;
    // creates a user with no links and an empty notification queue*
    user(string n, string p, string e)
    {
        name = n;
        password = p;
        email = e;
        next = NULL;
        prev = NULL;
        queuefront = NULL;
        queuerear = NULL;
    }
};
// notification queue node:
class notification
{
public:
    string message;
    notification *next;
};
// a single comment, stored in a singly linked list under each post*
class CommentNode
{
public:
    string username;
    string text;
    CommentNode *next;
    // stores who commented and what they wrote*
    CommentNode(string u, string t)
    {
        username = u;
        text = t;
        next = NULL;
    }
};
// a single post, stored as a node in the post linked list*
class PostNode
{
public:
    int postID;
    string content;
    string author;
    // total number of likes (the users who liked are kept in the likes registry)*
    int likes;
    PostNode *next;
    // head of this post's comment list*
    CommentNode *comments;
    // new posts start with 0 likes and no comments*
    PostNode(int id, string c, string a)
    {
        postID = id;
        content = c;
        author = a;
        likes = 0;
        next = NULL;
        comments = NULL;
    }
};
// linked list that stores all posts in the order they were created*
class PostLinkedList
{
public:
    PostNode *head;
    PostNode *tail;
    // id given to the next post, increases by 1 every time*
    int nextID;
    PostLinkedList()
    {
        head = NULL;
        tail = NULL;
        nextID = 1;
    }
    // create a new post and add it at the rear*
    void create_post(string content, string author)
    {
        PostNode *new_post =
            new PostNode(nextID, content, author);
        nextID++;
        // If list is empty*
        if (head == NULL)
        {
            head = tail = new_post;
        }
        // Otherwise append at rear*
        else
        {
            tail->next = new_post;
            tail = new_post;
        }
        cout << "post created successfully" << endl;
    }
    // bubble sort, the most liked posts move to the front*
    void sort_by_likes()
    {
        if (head == NULL || head->next == NULL)
        {
            return;
        }
        bool swapped;
        do
        {
            swapped = false;
            PostNode *current = head;
            while (current->next != NULL)
            {
                if (current->likes < current->next->likes)
                {
                    swap(current->postID, current->next->postID);
                    swap(current->content, current->next->content);
                    swap(current->author, current->next->author);
                    swap(current->likes, current->next->likes);
                    swap(current->comments, current->next->comments);
                    swapped = true;
                }
                current = current->next;
            }
        } while (swapped);
        cout << "Posts sorted by likes!";
    }
    // delete a post from the list, only its author is allowed to*
bool delete_post(int id, string username)
    {
        if (head == NULL)
        {
            cout << "No posts available";
            return false;
        }
        PostNode *current = head;
        PostNode *previous = NULL;
        // Search for post*
        while (current != NULL && current->postID != id)
        {
            previous = current;
            current = current->next;
        }
        // Post not found*
        if (current == NULL)
        {
            cout << "Post not found";
            return false;
        }
        // Check ownership*
        if (current->author != username)
        {
            cout << "You can only delete your own posts";
            return false;
        }
        // when deleting first post*
        if (current == head)
        {
            head = head->next;
            if (head == NULL)
            {
                tail = NULL;
            }
        }
        // when deleting middle or last post*
        else
        {
            previous->next = current->next;
            if (current == tail)
            {
                tail = previous;
            }
        }
        // free all the comments of the deleted post*
        CommentNode *comment = current->comments;
        while (comment != NULL)
        {
            CommentNode *temp = comment;
            comment = comment->next;
            delete temp;
        }
        delete current;
        cout << "Post has been deleted , by: " << username << endl;
        return true;
    }
    // display every post in the list*
    void display_posts()
    {
        PostNode *temp = head;
        if (temp == NULL)
        {
            cout << "No posts available" << endl;
            return;
        }
        while (temp != NULL)
        {
            cout << "\npost ID: " << temp->postID;
            cout << "\nauthor: " << temp->author;
            cout << "\ncontent: " << temp->content;
            cout << "\nlikes: " << temp->likes;
            cout << endl
                 << endl;
            temp = temp->next;
        }
    }
    // find the post and add a comment at the rear of its comment list*
    void add_comment(int id, string username, string text)
    {
        PostNode *post = head;
        while (post != NULL && post->postID != id)
        {
            post = post->next;
        }
        if (post == NULL)
        {
            cout << "Post not found" << endl;
            return;
        }
        CommentNode *newComment = new CommentNode(username, text);
        // if there are no comments yet, this becomes the first one*
        if (post->comments == NULL)
        {
            post->comments = newComment;
        }
        else
        {
            // otherwise go to the last comment and append*
            CommentNode *temp = post->comments;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newComment;
        }
        cout << "Comment added successfully" << endl;
    }
    // display all the comments of a post*
    void display_comments(int id)
    {
        PostNode *post = head;
        while (post != NULL && post->postID != id)
        {
            post = post->next;
        }
        if (post == NULL)
        {
            cout << "Post not found" << endl;
            return;
        }
        if (post->comments == NULL)
        {
            cout << "No comments on this post" << endl;
            return;
        }
        CommentNode *temp = post->comments;
        cout << "\nComments on post " << id << ":" << endl;
        while (temp != NULL)
        {
            cout << temp->username << ": " << temp->text << endl;
            temp = temp->next;
        }
    }
};
// adjacency list node representing one friend inside another user's friend list
class FriendNode
{
public:
    string username;
    FriendNode *next;
    FriendNode(string name)
    {
        username = name;
        next = NULL;
    }
};
// the user in the graph
class GraphVertex
{
public:
    string username;
    FriendNode *friends; // points to the users' friends list
    GraphVertex *next; // points to the next user in the graph (the next vertex)
    bool visited;
    GraphVertex(string name)
    {
        username = name;
        friends = NULL;
        next = NULL;
        visited = false;
    }
};
// node used by the BFS queue, holds a pointer to a graph vertex*
class TraversalNode
{
public:
    GraphVertex *vertex;
    TraversalNode *next;
    TraversalNode(GraphVertex *v)
    {
        vertex = v;
        next = NULL;
    }
};
// simple linked list queue (fifo) used by the BFS traversal*
class GraphQueue
{
private:
    // remove from the front, add at the rear*
    TraversalNode *front;
    TraversalNode *rear;
public:
    GraphQueue()
    {
        front = NULL;
        rear = NULL;
    }
    // queue is empty when front is NULL*
    bool is_empty()
    {
        return front == NULL;
    }
    // add a vertex at the rear*
    void enqueue(GraphVertex *v)
    {
        TraversalNode *newNode = new TraversalNode(v);
        if (rear == NULL)
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
    }
    // remove and return the vertex at the front*
    GraphVertex *dequeue()
    {
        if (is_empty())
        {
            return NULL;
        }
        TraversalNode *temp = front;
        GraphVertex *v = temp->vertex;
        front = front->next;
        if (front == NULL)
        {
            rear = NULL;
        }
        delete temp;
        return v;
    }
    // free any nodes left in the queue*
    ~GraphQueue()
    {
        while (!is_empty())
        {
            dequeue();
        }
    }
};
// friendNode and graphVertex classes combined to form the graph
class AdjacencyListGraph
{
public:
    GraphVertex *head;
    AdjacencyListGraph()
    {
        head = NULL;
    }
    // add a new user to the graph*
    void add_vertex(string username)
    {
        if (find_vertex(username) != NULL)
        {
            cout << "User already exists in graph" << endl;
            return;
        }
        GraphVertex *new_vertex = new GraphVertex(username);
        if (head == NULL)
        {
            head = new_vertex;
        }
        else
        {
            GraphVertex *temp = head;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = new_vertex;
        }
        cout << username << " added to graph" << endl;
    }
    // search for a particular user in the graph*
    GraphVertex *find_vertex(string username)
    {
        GraphVertex *temp = head;
        while (temp != NULL)
        {
            if (temp->username == username)
            {
                return temp;
            }
            temp = temp->next;
        }
        return NULL;
    }
    // connect two users as friends using an undirected edge*
    void add_friend(string user1, string user2)
    {
        GraphVertex *first = find_vertex(user1);
        GraphVertex *second = find_vertex(user2);
        if (first == NULL || second == NULL)
        {
            cout << "User does not exist" << endl;
            return;
        }
        if (user1 == user2)
        {
            cout << "Cannot add yourself as a friend" << endl;
            return;
        }
        if (is_friend(user1, user2))
        {
            cout << "Already friends" << endl;
            return;
        }
        FriendNode *new_friend1 = new FriendNode(user2);
        // insert at beginning cause O(1)*
        new_friend1->next = first->friends;
        first->friends = new_friend1;
        FriendNode *new_friend2 = new FriendNode(user1);
        new_friend2->next = second->friends;
        second->friends = new_friend2;
        cout << user1 << " and " << user2 << " are now friends" << endl;
    }
    // check if two users already friends*
    bool is_friend(string user1, string user2)
    {
        GraphVertex *first = find_vertex(user1);
        if (first == NULL)
        {
            return false;
        }
        FriendNode *temp = first->friends;
        // only one friend list is traversed, since they are connected by an undirected edge*
        while (temp != NULL)
        {
            if (temp->username == user2)
            {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }
    // display all friends of a user*
    void display_friends(string username)
    {
        GraphVertex *user = find_vertex(username);
        if (user == NULL)
        {
            cout << "User not found" << endl;
            return;
        }
        FriendNode *temp = user->friends;
        if (temp == NULL)
        {
            cout << username << " has no friends yet" << endl;
            return;
        }
        cout << "\nFriends of " << username << ":" << endl;
        while (temp != NULL)
        {
            cout << temp->username << endl;
            temp = temp->next;
        }
    }
    // remove friendship between two users (using doubly linked list concepts)*
    void remove_friend(string user1, string user2)
    {
        GraphVertex *first = find_vertex(user1);
        GraphVertex *second = find_vertex(user2);
        if (first == NULL || second == NULL)
        {
            cout << "User does not exist" << endl;
            return;
        }
        if (!is_friend(user1, user2))
        {
            cout << "These users are not friends" << endl;
            return;
        }
        // remove user2 from user1's friend list*
        FriendNode *current = first->friends;
        FriendNode *previous = NULL;
        while (current != NULL && current->username != user2)
        {
            previous = current;
            current = current->next;
        }
        if (current != NULL)
        {
            if (previous == NULL)
            {
                first->friends = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            delete current;
        }
        // remove user1 from user2's friend list*
        current = second->friends;
        previous = NULL;
        while (current != NULL && current->username != user1)
        {
            previous = current;
            current = current->next;
        }
        if (current != NULL)
        {
            if (previous == NULL)
            {
                second->friends = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            delete current;
        }
        cout << "Friendship removed successfully" << endl;
    }
    // Breadth First Search traversal of the graph*
    void BFS(string start)
    {
        GraphVertex *startVertex = find_vertex(start);
        if (startVertex == NULL)
        {
            cout << "User not found.\n";
            return;
        }
        // Reset visited status for all users*
        GraphVertex *temp = head;
        while (temp != NULL)
        {
            temp->visited = false;
            temp = temp->next;
        }
        GraphQueue q;
        startVertex->visited = true;
        q.enqueue(startVertex);
        cout << "\nBFS Traversal starting from " << start << ":\n";
        while (!q.is_empty())
        {
            GraphVertex *current = q.dequeue();
            cout << current->username << " ";
            FriendNode *friendTemp = current->friends;
            while (friendTemp != NULL)
            {
                GraphVertex *friendVertex = find_vertex(friendTemp->username);
                if (friendVertex != NULL && !friendVertex->visited)
                {
                    friendVertex->visited = true;
                    q.enqueue(friendVertex);
                }
                friendTemp = friendTemp->next;
            }
        }
        cout << endl;
    }
    // suggest users who have mutual friends with this user*
    void recommend_friends(string username)
    {
        GraphVertex *user = find_vertex(username);
        if (user == NULL)
        {
            cout << "User not found.\n";
            return;
        }
        GraphVertex *candidate = head;
        bool found = false;
        cout << "\nFriend recommendations for " << username << ":\n";
        while (candidate != NULL)
        {
            // Skip the user and existing friends*
            if (candidate != user && !is_friend(username, candidate->username))
            {
                int mutualCount = 0;
                FriendNode *myFriend = user->friends;
                // Check each of the user's friends*
                while (myFriend != NULL)
                {
                    // Check whether candidate has this friend too*
                    if (is_friend(candidate->username, myFriend->username))
                    {
                        mutualCount++;
                    }
                    myFriend = myFriend->next;
                }
                if (mutualCount > 0)
                {
                    cout << candidate->username << " - " << mutualCount
                         << " mutual friend(s)\n";
                    found = true;
                }
            }
            candidate = candidate->next;
        }
        if (!found)
        {
            cout << "No recommendations available.\n";
        }
    }
    // write all friendships to file*
    void save_friendships()
    {
        ofstream file("friendships.txt");
        GraphVertex *current = head;
        while (current != NULL)
        {
            FriendNode *temp = current->friends;
            while (temp != NULL)
            {
                // Save each undirected friendship only once*
                if (current->username < temp->username)
                {
                    file << quoted(current->username) << " "
                         << quoted(temp->username) << endl;
                }
                temp = temp->next;
            }
            current = current->next;
        }
        file.close();
        cout << "Friendships saved successfully" << endl;
    }
    // read friendships from file and rebuild the graph*
    void load_friendships()
    {
        ifstream file("friendships.txt");
        string user1, user2;
        while (file >> quoted(user1) >> quoted(user2))
        {
            if (!is_friend(user1, user2))
            {
                add_friend(user1, user2);
            }
        }
        file.close();
    }
    // Free all graph vertices and their friend lists*
    ~AdjacencyListGraph()
    {
        while (head != NULL)
        {
            GraphVertex *temp = head;
            head = head->next;
            while (temp->friends != NULL)
            {
                FriendNode *friendTemp = temp->friends;
                temp->friends = temp->friends->next;
                delete friendTemp;
            }
            delete temp;
        }
    }
};
// request node for pending friend requests
class RequestNode
{
public:
    string sender;
    string receiver;
    RequestNode *next;
    RequestNode(string s, string r)
    {
        sender = s;
        receiver = r;
        next = NULL;
    }
};
// request list to store pending friend requests
class RequestList
{
private:
    RequestNode *head;
    // find a particular request*
    RequestNode *find_request(string sender, string receiver)
    {
        RequestNode *temp = head;
        while (temp != NULL)
        {
            if (temp->sender == sender && temp->receiver == receiver)
            {
                return temp;
            }
            temp = temp->next;
        }
        return NULL;
    }
    // remove a request from the list*
    void remove_request(string sender, string receiver)
    {
        RequestNode *temp = head;
        RequestNode *previous = NULL;
        while (temp != NULL)
        {
            if (temp->sender == sender && temp->receiver == receiver)
            {
                if (previous == NULL)
                {
                    head = temp->next;
                }
                else
                {
                    previous->next = temp->next;
                }
                delete temp;
                return;
            }
            previous = temp;
            temp = temp->next;
        }
    }
public:
    RequestList()
    {
        head = NULL;
    }
    // 1. Send a friend request*
    void send_request(string sender, string receiver, AdjacencyListGraph &graph)
    {
        if (graph.find_vertex(sender) == NULL || graph.find_vertex(receiver) == NULL)
        {
            cout << "One or both users do not exist.\n";
            return;
        }
        if (sender == receiver)
        {
            cout << "You cannot send a request to yourself.\n";
            return;
        }
        if (graph.is_friend(sender, receiver))
        {
            cout << "These users are already friends.\n";
            return;
        }
        if (find_request(sender, receiver) != NULL ||
            find_request(receiver, sender) != NULL)
        {
            cout << "A request between these users already exists.\n";
            return;
        }
        RequestNode *newRequest = new RequestNode(sender, receiver);
        newRequest->next = head;
        head = newRequest;
        cout << "Friend request sent successfully.\n";
    }
    // 2. Display incoming requests*
    void display_incoming(string username)
    {
        RequestNode *temp = head;
        bool found = false;
        cout << "\nIncoming requests for " << username << ":\n";
        while (temp != NULL)
        {
            if (temp->receiver == username)
            {
                cout << "From: " << temp->sender << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found)
        {
            cout << "No incoming requests.\n";
        }
    }
    // 3. Accept a friend request*
    void accept_request(string receiver, string sender, AdjacencyListGraph &graph)
    {
        RequestNode *request = find_request(sender, receiver);
        if (request == NULL)
        {
            cout << "Friend request not found.\n";
            return;
        }
        if (graph.find_vertex(sender) == NULL || graph.find_vertex(receiver) == NULL)
        {
            cout << "One or both users do not exist in the graph.\n";
            return;
        }
        if (graph.is_friend(sender, receiver))
        {
            cout << "These users are already friends.\n";
            remove_request(sender, receiver);
            return;
        }
        // Add the friendship to the graph*
        graph.add_friend(sender, receiver);
        // Remove the accepted request*
        remove_request(sender, receiver);
        cout << "Friend request accepted. You are now friends!\n";
    }
    // 4. Reject a friend request*
    void reject_request(string receiver, string sender)
    {
        if (find_request(sender, receiver) == NULL)
        {
            cout << "Friend request not found.\n";
            return;
        }
        remove_request(sender, receiver);
        cout << "Friend request rejected.\n";
    }
    // write all pending requests to file*
    void save_requests() {
    ofstream file("requests.txt");
    RequestNode* temp = head;
    while (temp != NULL) {
        file << quoted(temp->sender) << " "
             << quoted(temp->receiver) << endl;
        temp = temp->next;
    }
    file.close();
    cout << "Friend requests saved successfully" << endl;
}
    // read pending requests from file*
void load_requests() {
    ifstream file("requests.txt");
    string sender, receiver;
    while (file >> quoted(sender) >> quoted(receiver)) {
        if (find_request(sender, receiver) == NULL) {
            RequestNode* newRequest =
                new RequestNode(sender, receiver);
            newRequest->next = head;
            head = newRequest;
        }
    }
    file.close();
}
    // Free all remaining pending requests*
    ~RequestList()
    {
        while (head != NULL)
        {
            RequestNode *temp = head;
            head = head->next;
            delete temp;
        }
    }
};
// one like, stores which user liked which post*
class LikeNode
{
public:
    int postID;
    string username;
    LikeNode *next;
    LikeNode(int id, string u)
    {
        postID = id;
        username = u;
        next = NULL;
    }
};
// linked list of all likes, used to stop duplicate likes*
class LikesRegistry
{
private:
    LikeNode *head;
public:
    LikesRegistry()
    {
        head = NULL;
    }
    // check if a user has already liked a post*
    bool has_liked(int id, string username)
    {
        LikeNode *temp = head;
        while (temp != NULL)
        {
            if (temp->postID == id &&
                temp->username == username)
            {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }
    // add a new like at the beginning cause O(1)*
    void add_like(int id, string username)
    {
        if (has_liked(id, username))
        {
            cout << "You have already liked this post" << endl;
            return;
        }
        LikeNode *newLike = new LikeNode(id, username);
        newLike->next = head;
        head = newLike;
    }
    // remove all likes of a post when it is deleted*
    void remove_post_likes(int id)
    {
        LikeNode *current = head;
        LikeNode *previous = NULL;
        while (current != NULL)
        {
            if (current->postID == id)
            {
                LikeNode *toDelete = current;
                if (previous == NULL)
                {
                    head = current->next;
                }
                else
                {
                    previous->next = current->next;
                }
                current = current->next;
                delete toDelete;
            }
            else
            {
                previous = current;
                current = current->next;
            }
        }
    }
    // display the users who liked a post*
    void display_likes(int id)
    {
        LikeNode *temp = head;
        bool found = false;
        cout << "\nUsers who liked post " << id << ":" << endl;
        while (temp != NULL)
        {
            if (temp->postID == id)
            {
                cout << "- " << temp->username << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found)
        {
            cout << "No likes yet." << endl;
        }
    }
    // write all likes to file*
    void save_likes()
    {
        ofstream file("likes.txt");
        LikeNode *temp = head;
        while (temp != NULL)
        {
            file << temp->postID << " "
                 << quoted(temp->username) << endl;
            temp = temp->next;
        }
        file.close();
    }
    // read likes from file*
    void load_likes()
    {
        ifstream file("likes.txt");
        int id;
        string username;
        while (file >> id >> quoted(username))
        {
            if (!has_liked(id, username))
            {
                add_like(id, username);
            }
        }
        file.close();
    }
    // free all like nodes*
    ~LikesRegistry()
    {
        while (head != NULL)
        {
            LikeNode *temp = head;
            head = head->next;
            delete temp;
        }
    }
};
// main class, ties together the users, posts, friendships, requests and likes*
class social_network
{
public:
    // doubly linked list of all registered users*
    user *head;
    user *tail;
    // the user who is currently logged in (NULL if nobody is)*
    user *active_user;
    // the other data structures used by the network*
    PostLinkedList posts;
    AdjacencyListGraph graph;
    RequestList requests;
    LikesRegistry likesRegistry;
    social_network()
    {
        head = NULL;
        tail = NULL;
        active_user = NULL;
    }
    // create a post as the logged in user*
    void create_post(string content)
    {
        if (active_user == NULL)
        {
            cout << "Please login first";
            return;
        }
        posts.create_post(content, active_user->name);
    }
    // display all posts*
    void display_posts()
    {
        posts.display_posts();
    }
    // sort all posts by number of likes*
    void sort_posts()
    {
        posts.sort_by_likes();
    }
    // log in after checking the username and password*
    bool login(string n, string p)
    {
        if (active_user != NULL)
        {
            cout << "Someone is already logged in" << endl;
            return false;
        }
        user *found = search(n);
        if (found == NULL)
        {
            cout << "User does not exist" << endl;
            return false;
        }
        if (found->password != p)
        {
            cout << "Incorrect password" << endl;
            return false;
        }
        active_user = found;
        cout << "Login successful" << endl;
        return true;
    }
    // log out the active user*
    void logout()
    {
        if (active_user == NULL)
        {
            cout << "No user is logged in" << endl;
            return;
        }
        active_user = NULL;
        cout << "Logged out" << endl;
    }
    // search for a user by name in the users list*
    user *search(string target_name)
    {
        user *current = head;
        while (current != NULL)
        {
            if (current->name == target_name)
            {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }
    // register a new user, no duplicate usernames allowed*
    bool register_user(string n, string p, string e)
    {
        // prevent duplication*
        if (search(n) != NULL)
        {
            cout << "user already exists" << endl;
            return false;
        }
        user *new_user = new user(n, p, e);
        // append to rear*
        if (head == NULL)
        {
            head = tail = new_user;
        }
        else
        {
            tail->next = new_user;
            new_user->prev = tail;
            tail = new_user;
        }
        // every new user is also added to the friendship graph*
        graph.add_vertex(n);
        return true;
    }
    // fifo - notification*
    // append new alert to queue rear*
    void enq_notification(user *targetUser, string alert_msg)
    {
        if (targetUser == NULL)
        {
            return;
        }
        notification *new_notification = new notification();
        new_notification->message = alert_msg;
        new_notification->next = NULL;
        if (targetUser->queuefront == NULL)
        {
            targetUser->queuefront = targetUser->queuerear = new_notification;
        }
        else
        {
            targetUser->queuerear->next = new_notification;
            targetUser->queuerear = new_notification;
        }
    }
    // remove the oldest alert from the queue front and display it*
    void dequeue_notif(user *active_user)
    {
        if (active_user == NULL || active_user->queuefront == NULL)
        {
            cout << "no unread alerts";
            return;
        }
        notification *temp = active_user->queuefront;
        cout << "new alert: " << temp->message << endl;
        active_user->queuefront = active_user->queuefront->next;
        if (active_user->queuefront == NULL)
        {
            active_user->queuerear = NULL;
        }
        delete temp;
    }
    // display all unread alerts from front to rear*
    void check_inbox(user *active_user)
    {
        if (active_user == NULL || active_user->queuefront == NULL)
        {
            cout << "No unread alerts" << endl;
            return;
        }
        notification *temp = active_user->queuefront;
        while (temp != NULL)
        {
            cout << temp->message << endl;
            temp = temp->next;
        }
    }
    // delete a post and also remove its likes*
    void delete_post(int id)
    {
        if (active_user == NULL)
        {
            cout << "Please login first";
            return;
        }
        if (posts.delete_post(id, active_user->name))
        {
            likesRegistry.remove_post_likes(id);
        }
    }
    // change the content of a post, only its author is allowed to*
    void edit_post(int id, string newContent)
    {
        if (active_user == NULL)
        {
            cout << "Please login first" << endl;
            return;
        }
        PostNode *temp = posts.head;
        while (temp != NULL && temp->postID != id)
        {
            temp = temp->next;
        }
        if (temp == NULL)
        {
            cout << "Post not found" << endl;
            return;
        }
        if (temp->author != active_user->name)
        {
            cout << "You can only edit your own posts" << endl;
            return;
        }
        temp->content = newContent;
        cout << "Post updated successfully!" << endl;
    }
    // show the users who liked a post*
    void view_likes(int id)
    {
        likesRegistry.display_likes(id);
    }
    // like a post (not your own, and only once), then notify the author*
    void like_post(int id)
    {
        if (active_user == NULL)
        {
            cout << "Please login first";
            return;
        }
        PostNode *temp = posts.head;
        while (temp != NULL && temp->postID != id)
        {
            temp = temp->next;
        }
        if (temp == NULL)
        {
            cout << "Post not found";
            return;
        }
        if (temp->author == active_user->name)
        {
            cout << "You cannot like your own post";
            return;
        }
        if (likesRegistry.has_liked(id, active_user->name))
        {
            cout << "You have already liked this post";
            return;
        }
        // record the like, increase the count and send an alert to the author*
        likesRegistry.add_like(id, active_user->name);
        temp->likes++;
        user *author = search(temp->author);
        if (author != NULL)
        {
            string msg = active_user->name +
                         " liked your post: " + temp->content;
            enq_notification(author, msg);
        }
        cout << "Post liked successfully";
    }
    // show all posts and ask which one to like*
    void choose_post_to_like()
    {
        if (active_user == NULL)
        {
            cout << "Please login first" << endl;
            return;
        }
        if (posts.head == NULL)
        {
            cout << "No posts available" << endl;
            return;
        }
        // Display all posts*
        cout << "\n===== AVAILABLE POSTS =====\n";
        posts.display_posts();
        // Ask user to choose*
        int id;
        cout << "\nEnter Post ID you want to like: ";
        cin >> id;
        // Like selected post*
        like_post(id);
    }
    // file handling:*
    // save users, notifications, posts, friendships, requests, likes and comments*
    void save_data()
    {
        ofstream userFile("users.txt");
        ofstream postFile("posts.txt");
        ofstream notifFile("notifications.txt");
        if (!userFile || !postFile || !notifFile)
        {
            cout << "Error opening files";
            return;
        }
        // Save users*
        user *u = head;
        while (u != NULL)
        {
            userFile << quoted(u->name) << " "
                     << quoted(u->password) << " "
                     << quoted(u->email) << endl;
            // Save this user's pending notifications*
            notification *n = u->queuefront;
            while (n != NULL)
            {
                notifFile << quoted(u->name) << " "
                          << quoted(n->message) << endl;
                n = n->next;
            }
            u = u->next;
        }
        // Save posts*
        PostNode *p = posts.head;
        while (p != NULL)
        {
            postFile << p->postID << " "
                     << quoted(p->content) << " "
                     << quoted(p->author) << " "
                     << p->likes << endl;
            p = p->next;
        }
        cout << "Data saved successfully!";
        graph.save_friendships();
        requests.save_requests();
        likesRegistry.save_likes();
        save_comments();
    }
    // load everything back from the files when the program starts*
    void load_data()
    {
        ifstream userFile("users.txt");
        ifstream postFile("posts.txt");
        ifstream notifFile("notifications.txt");
        string name, password, email;
        // Load users*
        while (userFile >> quoted(name) >> quoted(password) >> quoted(email))
        {
            user *new_user = new user(name, password, email);
            // every loaded user also needs a vertex in the friendship graph*
            graph.add_vertex(name);
            if (head == NULL)
            {
                head = tail = new_user;
            }
            else
            {
                tail->next = new_user;
                new_user->prev = tail;
                tail = new_user;
            }
        }
        // Load posts*
        int id, likes;
        string content, author;
        while (postFile >> id >> quoted(content) >> quoted(author) >> likes)
        {
            PostNode *new_post = new PostNode(id, content, author);
            new_post->likes = likes;
            if (posts.head == NULL)
            {
                posts.head = posts.tail = new_post;
            }
            else
            {
                posts.tail->next = new_post;
                posts.tail = new_post;
            }
            if (id >= posts.nextID)
            {
                posts.nextID = id + 1;
            }
        }
        // Load pending notifications*
        string username, message;
        while (notifFile >> quoted(username) >> quoted(message))
        {
            user *target = search(username);
            if (target != NULL)
            {
                enq_notification(target, message);
            }
        }
        // Load friendships, requests, likes and comments*
        // (users and posts must be loaded first, since these refer to them)
        graph.load_friendships();
        requests.load_requests();
        likesRegistry.load_likes();
        load_comments();
        cout << "Data loaded successfully!";
    }
    // send a friend request from the active user*
    void send_friend_request(string receiver)
    {
        if (active_user == NULL)
        {
            cout << "Please login first" << endl;
            return;
        }
        requests.send_request(active_user->name, receiver, graph);
    }
    // show the incoming friend requests of the active user*
    void view_friend_requests()
    {
        if (active_user == NULL)
        {
            cout << "Please login first" << endl;
            return;
        }
        requests.display_incoming(active_user->name);
    }
    // accept a request, both users become friends*
    void accept_friend_request(string sender)
    {
        if (active_user == NULL)
        {
            cout << "Please login first" << endl;
            return;
        }
        requests.accept_request(active_user->name, sender, graph);
    }
    // reject a request and remove it from the list*
    void reject_friend_request(string sender)
    {
        if (active_user == NULL)
        {
            cout << "Please login first" << endl;
            return;
        }
        requests.reject_request(active_user->name, sender);
    }
    // comment on a post as the active user*
    void comment_on_post(int id, string text)
    {
        if (active_user == NULL)
        {
            cout << "Please login first" << endl;
            return;
        }
        posts.add_comment(id, active_user->name, text);
    }
    // show the comments of a post*
    void view_comments(int id)
    {
        posts.display_comments(id);
    }
    // write all comments to file*
    void save_comments() {
    ofstream file("comments.txt");
    PostNode* post = posts.head;
    while (post != NULL) {
        CommentNode* comment = post->comments;
        while (comment != NULL) {
            file << post->postID << " "
                 << quoted(comment->username) << " "
                 << quoted(comment->text) << endl;
            comment = comment->next;
        }
        post = post->next;
    }
    file.close();
}
    // read comments from file and attach them to their posts*
void load_comments() {
    ifstream file("comments.txt");
    int id;
    string username, text;
    while (file >> id
                >> quoted(username)
                >> quoted(text)) {
        posts.add_comment(id, username, text);
    }
    file.close();
}
};
// keep asking until the user enters a valid number*
int read_choice()
{
    int choice;

    while (!(cin >> choice))
    {
        cout << "Please enter a number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    cin.ignore(10000, '\n');
    return choice;
}

// show a prompt and read a full line of text*
string read_text(string prompt)
{
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

// print one post inside a neat box*
void show_post(PostNode *post)
{
    if (post == NULL)
    {
        return;
    }

    cout << "\n----------------------------------" << endl;
    cout << "Post ID: " << post->postID << endl;
    cout << "Author: " << post->author << endl;
    cout << "Content: " << post->content << endl;
    cout << "Likes: " << post->likes << endl;
    cout << "----------------------------------" << endl;
}

// menu for managing the logged in user's own posts*
void my_posts_menu(social_network &app)
{
    int choice;

    do
    {
        cout << "\n========== MY POSTS ==========" << endl;
        cout << "1. Upload a Post" << endl;
        cout << "2. View My Posts" << endl;
        cout << "3. Edit a Post" << endl;
        cout << "4. Delete a Post" << endl;
        cout << "5. View Likes on a Post" << endl;
        cout << "6. View Comments on a Post" << endl;
        cout << "7. Comment on My Post" << endl;
        cout << "8. Back to Main Menu" << endl;
        cout << "Enter choice: ";

        choice = read_choice();

        if (choice == 1)
        {
            string content = read_text("Write your post: ");
            if (!content.empty())
            {
                app.create_post(content);
            }
            else
            {
                cout << "Post cannot be empty." << endl;
            }
        }
        else if (choice == 2)
        {
            // list only the posts written by the logged in user*
            PostNode *temp = app.posts.head;
            bool found = false;

            while (temp != NULL)
            {
                if (temp->author == app.active_user->name)
                {
                    show_post(temp);
                    found = true;
                }
                temp = temp->next;
            }

            if (!found)
            {
                cout << "You have not uploaded any posts yet." << endl;
            }
        }
        // options 3 to 7 all need a post ID*
        else if (choice >= 3 && choice <= 7)
        {
            int id;
            cout << "Enter Post ID: ";
            id = read_choice();

            if (choice == 3)
            {
                string content = read_text("Enter the new post content: ");
                app.edit_post(id, content);
            }
            else if (choice == 4)
            {
                app.delete_post(id);
            }
            else if (choice == 5)
            {
                app.view_likes(id);
            }
            else if (choice == 6)
            {
                app.view_comments(id);
            }
            else if (choice == 7)
            {
                string comment = read_text("Write your comment: ");
                app.comment_on_post(id, comment);
            }
        }
        else if (choice != 8)
        {
            cout << "Invalid choice." << endl;
        }

    } while (choice != 8);
}

// shows other users' posts one by one so they can be liked, commented on, etc*
void explore_feed_menu(social_network &app)
{
    int choice;

    while (true)
    {
        PostNode *current = app.posts.head;

        // Find the first post that does not belong to the logged-in user.
        while (current != NULL && current->author == app.active_user->name)
        {
            current = current->next;
        }

        if (current == NULL)
        {
            cout << "\nThere are no posts from other users yet." << endl;
            return;
        }

        bool leaveFeed = false;

        while (current != NULL && !leaveFeed)
        {
            if (current->author != app.active_user->name)
            {
                show_post(current);

                cout << "\n1. Like Post" << endl;
                cout << "2. Comment on Post" << endl;
                cout << "3. View Comments" << endl;
                cout << "4. Send Friend Request to Author" << endl;
                cout << "5. Next Post" << endl;
                cout << "6. Back to Main Menu" << endl;
                cout << "Enter choice: ";

                choice = read_choice();

                if (choice == 1)
                {
                    app.like_post(current->postID);
                }
                else if (choice == 2)
                {
                    string comment = read_text("Write your comment: ");
                    app.comment_on_post(current->postID, comment);
                }
                else if (choice == 3)
                {
                    app.view_comments(current->postID);
                }
                else if (choice == 4)
                {
                    app.send_friend_request(current->author);
                }
                else if (choice == 5)
                {
                    current = current->next;
                }
                else if (choice == 6)
                {
                    leaveFeed = true;
                }
                else
                {
                    cout << "Invalid choice." << endl;
                }
            }
            else
            {
                current = current->next;
            }
        }

        if (leaveFeed)
        {
            return;
        }

        if (current == NULL)
        {
            cout << "\nYou have reached the end of the feed." << endl;
            cout << "1. Start Feed Again" << endl;
            cout << "2. Back to Main Menu" << endl;
            cout << "Enter choice: ";

            choice = read_choice();

            if (choice == 2)
            {
                return;
            }
            else if (choice != 1)
            {
                cout << "Invalid choice. Returning to main menu." << endl;
                return;
            }
        }
    }
}

// show incoming requests and let the user accept or reject one*
void friend_requests_menu(social_network &app)
{
    app.view_friend_requests();

    string sender = read_text("\nEnter the username to respond to (or press Enter to go back): ");

    if (sender.empty())
    {
        return;
    }

    cout << "1. Accept" << endl;
    cout << "2. Reject" << endl;
    cout << "3. Cancel" << endl;
    cout << "Enter choice: ";

    int choice = read_choice();

    if (choice == 1)
    {
        app.accept_friend_request(sender);
    }
    else if (choice == 2)
    {
        app.reject_friend_request(sender);
    }
}

// search for a user by name and optionally send a friend request*
void search_users_menu(social_network &app)
{
    string username = read_text("Enter username to search: ");
    user *found = app.search(username);

    if (found == NULL)
    {
        cout << "User not found." << endl;
        return;
    }

    if (found->name == app.active_user->name)
    {
        cout << "This is your own profile." << endl;
        return;
    }

    cout << "\nUser found: " << found->name << endl;
    cout << "1. Send Friend Request" << endl;
    cout << "2. Back" << endl;
    cout << "Enter choice: ";

    int choice = read_choice();

    if (choice == 1)
    {
        app.send_friend_request(found->name);
    }
}

// main menu shown after login, runs until the user logs out*
void logged_in_menu(social_network &app)
{
    int choice;

    while (app.active_user != NULL)
    {
        cout << "\n========== SOCIAL NETWORK ==========" << endl;
        cout << "Welcome, " << app.active_user->name << "!" << endl;
        cout << "1. Manage My Posts" << endl;
        cout << "2. Explore Feed" << endl;
        cout << "3. View Friend Requests" << endl;
        cout << "4. View My Friends" << endl;
        cout << "5. Search Users" << endl;
        cout << "6. View Notifications" << endl;
        cout << "7. Logout" << endl;
        cout << "Enter choice: ";

        choice = read_choice();

        if (choice == 1)
        {
            my_posts_menu(app);
        }
        else if (choice == 2)
        {
            explore_feed_menu(app);
        }
        else if (choice == 3)
        {
            friend_requests_menu(app);
        }
        else if (choice == 4)
        {
            app.graph.display_friends(app.active_user->name);
        }
        else if (choice == 5)
        {
            search_users_menu(app);
        }
        else if (choice == 6)
        {
            app.check_inbox(app.active_user);
        }
        else if (choice == 7)
        {
            // save everything before logging out*
            app.save_data();
            app.logout();
        }
        else
        {
            cout << "Invalid choice. Try again." << endl;
        }
    }
}

// program starts here: register, login or exit*
int main()
{
    social_network app;

    // load the saved data from the files*
    app.load_data();

    int choice;

    while (true)
    {
        cout << "\n========== WELCOME TO SOCIAL NETWORK ==========" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter choice: ";

        choice = read_choice();

        if (choice == 1)
        {
            // register a new user*
            string username = read_text("Enter username: ");
            string password = read_text("Enter password: ");
            string email = read_text("Enter email: ");

            if (app.register_user(username, password, email))
            {
                cout << "Registration successful!" << endl;
                // save right after registering*
                app.save_data();
            }
        }
        else if (choice == 2)
        {
            // login with username and password*
            string username = read_text("Enter username: ");
            string password = read_text("Enter password: ");

            if (app.login(username, password))
            {
                logged_in_menu(app);
            }
        }
        else if (choice == 3)
        {
            // save everything before exiting*
            app.save_data();
            cout << "Thank you for using the Social Network System!" << endl;
            break;
        }
        else
        {
            cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}
