#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif
using namespace std;

// set to false if your console can't show emojis
const bool USE_EMOJI = true;

// ANSI colour codes (256 colour palette)
const string C_RESET = "\033[0m";
const string C_BOLD = "\033[1m";
const string C_DIM = "\033[2m";
const string C_PINK = "\033[38;5;213m";
const string C_PURPLE = "\033[38;5;141m";
const string C_CYAN = "\033[38;5;51m";
const string C_BLUE = "\033[38;5;75m";
const string C_GREEN = "\033[38;5;120m";
const string C_YELLOW = "\033[38;5;226m";
const string C_ORANGE = "\033[38;5;215m";
const string C_RED = "\033[38;5;203m";
const string C_GREY = "\033[38;5;245m";

// when true, status messages are hidden (used while loading files)
bool g_quiet = false;

// turn on colours + UTF-8 on Windows consoles
void init_console()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode))
    {
        SetConsoleMode(h, mode | 0x0004); // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    }
#endif
}

// returns the emoji followed by a space (or nothing if emojis are off)
string emo(string e)
{
    if (USE_EMOJI)
    {
        return e + " ";
    }
    return "";
}

// repeat a (UTF-8) string n times
string repeat(string s, int n)
{
    string out = "";
    for (int i = 0; i < n; i++)
    {
        out += s;
    }
    return out;
}

// every letter gets the next colour of the rainbow
string rainbow(string text)
{
    const string colours[] = {C_RED, C_ORANGE, C_YELLOW, C_GREEN, C_CYAN, C_BLUE, C_PURPLE, C_PINK};
    string out = C_BOLD;
    int k = 0;
    for (size_t i = 0; i < text.size(); i++)
    {
        if (text[i] == ' ')
        {
            out += text[i];
        }
        else
        {
            out += colours[k % 8] + string(1, text[i]);
            k++;
        }
    }
    return out + C_RESET;
}

// coloured status messages
void say(string colour, string icon, string msg)
{
    if (g_quiet)
    {
        return;
    }
    cout << "  " << colour << emo(icon) << msg << C_RESET << endl;
}
void ok(string msg) { say(C_GREEN, "✅", msg); }
void err(string msg) { say(C_RED, "💔", msg); }
void info(string msg) { say(C_CYAN, "💡", msg); }

// big boxed title (title should be plain ASCII so it lines up)
void header(string title, string colour)
{
    const int W = 40;
    int pad = W - (int)title.size();
    if (pad < 0)
    {
        pad = 0;
    }
    int left = pad / 2;
    int right = pad - left;
    cout << "\n"
         << colour << C_BOLD << "╔" << repeat("═", W) << "╗\n"
         << "║" << string(left, ' ') << title << string(right, ' ') << "║\n"
         << "╚" << repeat("═", W) << "╝" << C_RESET << endl;
}

// one numbered menu line
void menu_item(int n, string icon, string text, string colour)
{
    cout << "  " << colour << C_BOLD << "[" << n << "]" << C_RESET
         << " " << emo(icon) << text << endl;
}

// the "Enter choice" prompt
void prompt_choice()
{
    cout << "\n  " << C_YELLOW << C_BOLD << "➤ Enter choice: " << C_RESET;
}

// section divider
void divider(string colour)
{
    cout << colour << "  " << repeat("·", 40) << C_RESET << endl;
}

// the cute welcome banner
void show_banner()
{
    cout << "\n"
         << "  " << rainbow("*  .  *  .  *  .  *  .  *  .  *  .  *  .  *") << endl;
    cout << "  " << emo("🌸") << emo("✨") << rainbow("S O C I A L   N E T W O R K") << " " << emo("✨") << emo("🌸") << endl;
    cout << "  " << C_GREY << "      connect  ·  share  ·  sparkle" << C_RESET << endl;
    cout << "  " << rainbow("*  .  *  .  *  .  *  .  *  .  *  .  *  .  *") << endl;
}

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

// print one post as a pretty card
void print_post_card(PostNode *p)
{
    if (p == NULL)
    {
        return;
    }
    int commentCount = 0;
    CommentNode *c = p->comments;
    while (c != NULL)
    {
        commentCount++;
        c = c->next;
    }
    string line = repeat("─", 38);
    cout << "\n"
         << C_PURPLE << "  ╭" << line << C_RESET << endl;
    cout << C_PURPLE << "  │ " << C_RESET << emo("📌") << C_BOLD << C_PINK << "Post #" << p->postID << C_RESET
         << C_GREY << "  ·  by " << C_RESET << C_CYAN << C_BOLD << p->author << C_RESET << endl;
    cout << C_PURPLE << "  │" << C_RESET << endl;
    cout << C_PURPLE << "  │ " << C_RESET << p->content << endl;
    cout << C_PURPLE << "  │" << C_RESET << endl;
    cout << C_PURPLE << "  │ " << C_RESET << emo("💖") << C_RED << p->likes << " likes" << C_RESET
         << "   " << emo("💬") << C_BLUE << commentCount << " comments" << C_RESET << endl;
    cout << C_PURPLE << "  ╰" << line << C_RESET << endl;
}

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
        ok("Post created successfully! Time to shine ✨");
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
        ok("Posts sorted by likes!");
    }
    // delete a post from the list, only its author is allowed to*
    bool delete_post(int id, string username)
    {
        if (head == NULL)
        {
            err("No posts available");
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
            err("Post not found");
            return false;
        }
        // Check ownership*
        if (current->author != username)
        {
            err("You can only delete your own posts");
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
        ok("Post has been deleted, by: " + username);
        return true;
    }
    // display every post in the list*
    void display_posts()
    {
        PostNode *temp = head;
        if (temp == NULL)
        {
            info("No posts available");
            return;
        }
        while (temp != NULL)
        {
            print_post_card(temp);
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
            err("Post not found");
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
        ok("Comment added successfully!");
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
            err("Post not found");
            return;
        }
        if (post->comments == NULL)
        {
            info("No comments on this post yet");
            return;
        }
        CommentNode *temp = post->comments;
        cout << "\n  " << C_BLUE << C_BOLD << emo("💬") << "Comments on post " << id << C_RESET << endl;
        divider(C_BLUE);
        while (temp != NULL)
        {
            cout << "  " << C_CYAN << C_BOLD << temp->username << C_RESET
                 << C_GREY << " says: " << C_RESET << temp->text << endl;
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
    GraphVertex *next;   // points to the next user in the graph (the next vertex)
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
            err("User already exists in graph");
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
            err("User does not exist");
            return;
        }
        if (user1 == user2)
        {
            err("Cannot add yourself as a friend");
            return;
        }
        if (is_friend(user1, user2))
        {
            info("Already friends");
            return;
        }
        FriendNode *new_friend1 = new FriendNode(user2);
        // insert at beginning cause O(1)*
        new_friend1->next = first->friends;
        first->friends = new_friend1;
        FriendNode *new_friend2 = new FriendNode(user1);
        new_friend2->next = second->friends;
        second->friends = new_friend2;
        ok(user1 + " and " + user2 + " are now friends");
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
            err("User not found");
            return;
        }
        FriendNode *temp = user->friends;
        if (temp == NULL)
        {
            info(username + " has no friends yet. Go say hi!");
            return;
        }
        cout << "\n  " << C_PINK << C_BOLD << emo("👯") << "Friends of " << username << C_RESET << endl;
        divider(C_PINK);
        while (temp != NULL)
        {
            cout << "  " << C_PINK << emo("🌷") << C_RESET << temp->username << endl;
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
            err("User does not exist");
            return;
        }
        if (!is_friend(user1, user2))
        {
            err("These users are not friends");
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
        ok("Friendship removed successfully");
    }
    // Breadth First Search traversal of the graph*
    void BFS(string start)
    {
        GraphVertex *startVertex = find_vertex(start);
        if (startVertex == NULL)
        {
            err("User not found");
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
        cout << "\n  " << C_CYAN << C_BOLD << emo("🔭") << "BFS Traversal starting from " << start << C_RESET << endl;
        cout << "  ";
        while (!q.is_empty())
        {
            GraphVertex *current = q.dequeue();
            cout << C_CYAN << current->username << C_RESET << " ";
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
            err("User not found");
            return;
        }
        GraphVertex *candidate = head;
        bool found = false;
        cout << "\n  " << C_YELLOW << C_BOLD << emo("🔮") << "Friend recommendations for " << username << C_RESET << endl;
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
                    cout << "  " << C_YELLOW << emo("⭐") << C_RESET << candidate->username << " - "
                         << mutualCount << " mutual friend(s)" << endl;
                    found = true;
                }
            }
            candidate = candidate->next;
        }
        if (!found)
        {
            info("No recommendations available");
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
            err("One or both users do not exist");
            return;
        }
        if (sender == receiver)
        {
            err("You cannot send a request to yourself");
            return;
        }
        if (graph.is_friend(sender, receiver))
        {
            info("These users are already friends");
            return;
        }
        if (find_request(sender, receiver) != NULL ||
            find_request(receiver, sender) != NULL)
        {
            info("A request between these users already exists");
            return;
        }
        RequestNode *newRequest = new RequestNode(sender, receiver);
        newRequest->next = head;
        head = newRequest;
        ok("Friend request sent successfully!");
    }
    // 2. Display incoming requests*
    void display_incoming(string username)
    {
        RequestNode *temp = head;
        bool found = false;
        cout << "\n  " << C_ORANGE << C_BOLD << emo("💌") << "Incoming requests for " << username << C_RESET << endl;
        divider(C_ORANGE);
        while (temp != NULL)
        {
            if (temp->receiver == username)
            {
                cout << "  " << C_ORANGE << emo("💗") << C_RESET << "From: "
                     << C_CYAN << C_BOLD << temp->sender << C_RESET << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found)
        {
            info("No incoming requests");
        }
    }
    // 3. Accept a friend request*
    void accept_request(string receiver, string sender, AdjacencyListGraph &graph)
    {
        RequestNode *request = find_request(sender, receiver);
        if (request == NULL)
        {
            err("Friend request not found");
            return;
        }
        if (graph.find_vertex(sender) == NULL || graph.find_vertex(receiver) == NULL)
        {
            err("One or both users do not exist in the graph");
            return;
        }
        if (graph.is_friend(sender, receiver))
        {
            info("These users are already friends");
            remove_request(sender, receiver);
            return;
        }
        // Add the friendship to the graph*
        graph.add_friend(sender, receiver);
        // Remove the accepted request*
        remove_request(sender, receiver);
        ok("Friend request accepted. You are now friends! 🎉");
    }
    // 4. Reject a friend request*
    void reject_request(string receiver, string sender)
    {
        if (find_request(sender, receiver) == NULL)
        {
            err("Friend request not found");
            return;
        }
        remove_request(sender, receiver);
        ok("Friend request rejected");
    }
    // write all pending requests to file*
    void save_requests()
    {
        ofstream file("requests.txt");
        RequestNode *temp = head;
        while (temp != NULL)
        {
            file << quoted(temp->sender) << " "
                 << quoted(temp->receiver) << endl;
            temp = temp->next;
        }
        file.close();
    }
    // read pending requests from file*
    void load_requests()
    {
        ifstream file("requests.txt");
        string sender, receiver;
        while (file >> quoted(sender) >> quoted(receiver))
        {
            if (find_request(sender, receiver) == NULL)
            {
                RequestNode *newRequest =
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
            info("You have already liked this post");
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
        cout << "\n  " << C_RED << C_BOLD << emo("💖") << "Users who liked post " << id << C_RESET << endl;
        divider(C_RED);
        while (temp != NULL)
        {
            if (temp->postID == id)
            {
                cout << "  " << C_RED << emo("❤️") << C_RESET << temp->username << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found)
        {
            info("No likes yet");
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
            err("Please login first");
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
            info("Someone is already logged in");
            return false;
        }
        user *found = search(n);
        if (found == NULL)
        {
            err("User does not exist");
            return false;
        }
        if (found->password != p)
        {
            err("Incorrect password");
            return false;
        }
        active_user = found;
        ok("Login successful!");
        return true;
    }
    // log out the active user*
    void logout()
    {
        if (active_user == NULL)
        {
            info("No user is logged in");
            return;
        }
        active_user = NULL;
        ok("Logged out. See you soon! 👋");
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
            err("User already exists");
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
            info("No unread alerts");
            return;
        }
        notification *temp = active_user->queuefront;
        cout << "  " << C_YELLOW << emo("🔔") << "New alert: " << C_RESET << temp->message << endl;
        active_user->queuefront = active_user->queuefront->next;
        if (active_user->queuefront == NULL)
        {
            active_user->queuerear = NULL;
        }
        delete temp;
    }
    // count unread alerts (shown on the main menu)*
    int unread_count(user *u)
    {
        int count = 0;
        if (u == NULL)
        {
            return 0;
        }
        notification *temp = u->queuefront;
        while (temp != NULL)
        {
            count++;
            temp = temp->next;
        }
        return count;
    }
    // display all unread alerts from front to rear*
    void check_inbox(user *active_user)
    {
        if (active_user == NULL || active_user->queuefront == NULL)
        {
            info("No unread alerts. You're all caught up!");
            return;
        }
        cout << "\n  " << C_YELLOW << C_BOLD << emo("🔔") << "Your notifications" << C_RESET << endl;
        divider(C_YELLOW);
        notification *temp = active_user->queuefront;
        int n = 1;
        while (temp != NULL)
        {
            cout << "  " << C_YELLOW << n << "." << C_RESET << " " << temp->message << endl;
            temp = temp->next;
            n++;
        }
    }
    // delete a post and also remove its likes*
    void delete_post(int id)
    {
        if (active_user == NULL)
        {
            err("Please login first");
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
            err("Please login first");
            return;
        }
        PostNode *temp = posts.head;
        while (temp != NULL && temp->postID != id)
        {
            temp = temp->next;
        }
        if (temp == NULL)
        {
            err("Post not found");
            return;
        }
        if (temp->author != active_user->name)
        {
            err("You can only edit your own posts");
            return;
        }
        temp->content = newContent;
        ok("Post updated successfully!");
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
            err("Please login first");
            return;
        }
        PostNode *temp = posts.head;
        while (temp != NULL && temp->postID != id)
        {
            temp = temp->next;
        }
        if (temp == NULL)
        {
            err("Post not found");
            return;
        }
        if (temp->author == active_user->name)
        {
            info("You cannot like your own post");
            return;
        }
        if (likesRegistry.has_liked(id, active_user->name))
        {
            info("You have already liked this post");
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
        ok("Post liked successfully! 💖");
    }
    // show all posts and ask which one to like*
    void choose_post_to_like()
    {
        if (active_user == NULL)
        {
            err("Please login first");
            return;
        }
        if (posts.head == NULL)
        {
            info("No posts available");
            return;
        }
        // Display all posts*
        header("AVAILABLE POSTS", C_PINK);
        posts.display_posts();
        // Ask user to choose*
        int id;
        cout << "\n  " << C_YELLOW << "Enter Post ID you want to like: " << C_RESET;
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
            err("Error opening files");
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
        graph.save_friendships();
        requests.save_requests();
        likesRegistry.save_likes();
        save_comments();
    }
    // load everything back from the files when the program starts*
    void load_data()
    {
        // keep the console clean while rebuilding everything from files
        g_quiet = true;
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
        g_quiet = false;
    }
    // send a friend request from the active user*
    void send_friend_request(string receiver)
    {
        if (active_user == NULL)
        {
            err("Please login first");
            return;
        }
        requests.send_request(active_user->name, receiver, graph);
    }
    // show the incoming friend requests of the active user*
    void view_friend_requests()
    {
        if (active_user == NULL)
        {
            err("Please login first");
            return;
        }
        requests.display_incoming(active_user->name);
    }
    // accept a request, both users become friends*
    void accept_friend_request(string sender)
    {
        if (active_user == NULL)
        {
            err("Please login first");
            return;
        }
        requests.accept_request(active_user->name, sender, graph);
    }
    // reject a request and remove it from the list*
    void reject_friend_request(string sender)
    {
        if (active_user == NULL)
        {
            err("Please login first");
            return;
        }
        requests.reject_request(active_user->name, sender);
    }
    // comment on a post as the active user*
    void comment_on_post(int id, string text)
    {
        if (active_user == NULL)
        {
            err("Please login first");
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
    void save_comments()
    {
        ofstream file("comments.txt");
        PostNode *post = posts.head;
        while (post != NULL)
        {
            CommentNode *comment = post->comments;
            while (comment != NULL)
            {
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
    void load_comments()
    {
        ifstream file("comments.txt");
        int id;
        string username, text;
        while (file >> id
               >> quoted(username)
               >> quoted(text))
        {
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
        cout << "  " << C_RED << emo("🙈") << "Please enter a number: " << C_RESET;
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
    cout << "  " << C_CYAN << emo("✏️ ") << prompt << C_RESET;
    getline(cin, value);
    return value;
}

// menu for managing the logged in user's own posts*
void my_posts_menu(social_network &app)
{
    int choice;

    do
    {
        header("MY POSTS", C_PURPLE);
        menu_item(1, "📝", "Upload a Post", C_PINK);
        menu_item(2, "📖", "View My Posts", C_PURPLE);
        menu_item(3, "✏️ ", "Edit a Post", C_BLUE);
        menu_item(4, "🗑️ ", "Delete a Post", C_RED);
        menu_item(5, "💖", "View Likes on a Post", C_ORANGE);
        menu_item(6, "💬", "View Comments on a Post", C_CYAN);
        menu_item(7, "🗨️ ", "Comment on My Post", C_GREEN);
        menu_item(8, "🔙", "Back to Main Menu", C_GREY);
        prompt_choice();

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
                err("Post cannot be empty");
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
                    print_post_card(temp);
                    found = true;
                }
                temp = temp->next;
            }

            if (!found)
            {
                info("You have not uploaded any posts yet");
            }
        }
        // options 3 to 7 all need a post ID*
        else if (choice >= 3 && choice <= 7)
        {
            int id;
            cout << "  " << C_CYAN << emo("🔢") << "Enter Post ID: " << C_RESET;
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
            err("Invalid choice");
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
            info("There are no posts from other users yet");
            return;
        }

        bool leaveFeed = false;

        header("EXPLORE FEED", C_CYAN);

        while (current != NULL && !leaveFeed)
        {
            if (current->author != app.active_user->name)
            {
                print_post_card(current);

                cout << endl;
                menu_item(1, "💖", "Like Post", C_RED);
                menu_item(2, "💬", "Comment on Post", C_GREEN);
                menu_item(3, "👀", "View Comments", C_CYAN);
                menu_item(4, "💌", "Send Friend Request to Author", C_ORANGE);
                menu_item(5, "➡️ ", "Next Post", C_PURPLE);
                menu_item(6, "🔙", "Back to Main Menu", C_GREY);
                prompt_choice();

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
                    err("Invalid choice");
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
            cout << "\n  " << C_YELLOW << emo("🎈") << "You have reached the end of the feed!" << C_RESET << endl;
            menu_item(1, "🔄", "Start Feed Again", C_GREEN);
            menu_item(2, "🔙", "Back to Main Menu", C_GREY);
            prompt_choice();

            choice = read_choice();

            if (choice == 2)
            {
                return;
            }
            else if (choice != 1)
            {
                err("Invalid choice. Returning to main menu");
                return;
            }
        }
    }
}

// show incoming requests and let the user accept or reject one*
void friend_requests_menu(social_network &app)
{
    header("FRIEND REQUESTS", C_ORANGE);
    app.view_friend_requests();

    string sender = read_text("\n  Enter the username to respond to (or press Enter to go back): ");

    if (sender.empty())
    {
        return;
    }

    cout << endl;
    menu_item(1, "💚", "Accept", C_GREEN);
    menu_item(2, "💔", "Reject", C_RED);
    menu_item(3, "🔙", "Cancel", C_GREY);
    prompt_choice();

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
    header("SEARCH USERS", C_BLUE);
    string username = read_text("Enter username to search: ");
    user *found = app.search(username);

    if (found == NULL)
    {
        err("User not found");
        return;
    }

    if (found->name == app.active_user->name)
    {
        info("This is your own profile");
        return;
    }

    cout << "\n  " << C_GREEN << emo("🎯") << "User found: " << C_BOLD << found->name << C_RESET << endl;
    menu_item(1, "💌", "Send Friend Request", C_PINK);
    menu_item(2, "🔙", "Back", C_GREY);
    prompt_choice();

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
        header("SOCIAL NETWORK", C_PINK);
        cout << "  " << emo("🌸") << "Welcome, " << C_PINK << C_BOLD << app.active_user->name << C_RESET << "!" << endl
             << endl;

        int unread = app.unread_count(app.active_user);
        string notifLabel = "View Notifications";
        if (unread > 0)
        {
            notifLabel += " " + C_RED + C_BOLD + "(" + to_string(unread) + " new!)" + C_RESET;
        }

        menu_item(1, "📝", "Manage My Posts", C_PINK);
        menu_item(2, "🧭", "Explore Feed", C_CYAN);
        menu_item(3, "💌", "View Friend Requests", C_ORANGE);
        menu_item(4, "👯", "View My Friends", C_GREEN);
        menu_item(5, "🔍", "Search Users", C_BLUE);
        menu_item(6, "🔔", notifLabel, C_YELLOW);
        menu_item(7, "👋", "Logout", C_GREY);
        prompt_choice();

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
            info("Progress saved");
            app.logout();
        }
        else
        {
            err("Invalid choice. Try again");
        }
    }
}

// program starts here: register, login or exit*
int main()
{
    init_console();

    social_network app;

    // load the saved data from the files*
    app.load_data();

    int choice;

    show_banner();

    while (true)
    {
        header("WELCOME", C_PURPLE);
        menu_item(1, "🌟", "Register", C_GREEN);
        menu_item(2, "🔑", "Login", C_CYAN);
        menu_item(3, "🚪", "Exit", C_GREY);
        prompt_choice();

        choice = read_choice();

        if (choice == 1)
        {
            // register a new user*
            cout << "\n  " << C_GREEN << C_BOLD << emo("🌟") << "Create your account" << C_RESET << endl;
            string username = read_text("Enter username: ");
            string password = read_text("Enter password: ");
            string email = read_text("Enter email: ");

            if (app.register_user(username, password, email))
            {
                ok("Registration successful! Welcome aboard 🎉");
                // save right after registering*
                app.save_data();
            }
        }
        else if (choice == 2)
        {
            // login with username and password*
            cout << "\n  " << C_CYAN << C_BOLD << emo("🔑") << "Welcome back!" << C_RESET << endl;
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
            cout << "\n  " << emo("💖") << rainbow("Thank you for using the Social Network System!") << " " << emo("🌈") << endl
                 << endl;
            break;
        }
        else
        {
            err("Invalid choice. Try again");
        }
    }

    return 0;
}