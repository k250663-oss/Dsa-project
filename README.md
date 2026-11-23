Social Network System
A console-based social networking application developed in C++ for a
Data Structures and Algorithms (DSA) project. The program simulates core
social media features while demonstrating how manually implemented
linked lists, queues, graphs, and file handling can work together.
Project Overview
Users can register and log in, publish posts, interact with other users'
posts, manage friend requests, and maintain a friendship network.
Application data is saved in text files so that users, posts,
friendships, requests, comments, likes, and notifications can be
restored when the program is run again.
Features
Account Management
Register a new account with a username, password, and email address.
Log in and log out.
Search for registered users.
Posts and Feed
Upload posts containing text.
View and manage your own posts.
Edit or delete your own posts.
Browse other users' posts through the Explore Feed.
Like posts and view the users who liked your own posts.
Add comments and view comments on posts.
Prevent users from liking their own posts or liking the same post
more than once.
Friendship Network
Search for users and send friend requests.
View incoming friend requests.
Accept or reject friend requests.
View your friends.
Prevent self-requests, duplicate requests, and requests between
users who are already friends.
Maintain friendships as an undirected graph.
Notifications
Receive notifications for post likes and friend-request activity
where supported by the program.
View the notification inbox.
Store notifications in a custom FIFO queue.
Data Persistence
Save application data to text files.
Load saved data automatically when the application starts.
Preserve records between program executions.
Data Structures and Concepts
The project uses manually implemented data structures rather than STL
containers such as `vector`, `list`, or `map`.
---
Data structure / concept            Use in the project
---
Doubly linked list                  Stores registered user records
Singly linked list                  Stores posts, comments, friend
lists, friend requests, and like
records
Undirected graph (adjacency list)   Represents friendships between
users
Custom linked queue                 Stores notifications in FIFO order
Custom linked queue for traversal   Supports Breadth-First Search (BFS)
Searching                           Finds users, posts, friends, and
requests
Sorting                             Sorts posts by number of likes
File handling                       Saves and restores application data
Requirements
A C++ compiler that supports C++14 or later.
A terminal or command prompt.
No external libraries are required.
The source uses standard headers including `iostream`, `string`,
`fstream`, and `iomanip`.
How to Compile and Run
Open a terminal in the folder containing the source file.
Compile
Using g++:
``` bash
g++ -std=c++14 -Wall -Wextra -pedantic socialNetwork_menu.cpp -o socialNetwork
```
Run
On Windows (PowerShell or Command Prompt):
``` bash
.\socialNetwork.exe
```
On Linux or macOS:
``` bash
./socialNetwork
```
If your compiler creates an executable with a different name, run that
executable instead.
Using the Application
When the program starts, the welcome menu provides options to:
Register
Log in
Exit
After logging in, the main menu provides access to:
Manage My Posts --- upload, view, edit, and delete your posts;
view likes and comments; add comments.
Explore Feed --- browse other users' posts and interact with
them.
Friend Requests --- view incoming requests and accept or reject
them.
Friends --- view your current friends.
Search Users --- find users and send friend requests.
Notifications --- view notifications in your inbox.
Logout --- save data and return to the welcome menu.
Follow the on-screen prompts and enter the relevant menu number. Post
IDs are used when selecting a post to edit, delete, like, or comment on.
Data Files
The program creates and uses the following text files in its current
working directory:
File                  Information stored
---
`users.txt`           Usernames, passwords, and email addresses
`posts.txt`           Post IDs, post content, authors, and like counts
`friendships.txt`     User pairs representing accepted friendships
`requests.txt`        Pending friend requests
`likes.txt`           Post IDs and usernames of users who liked each post
`comments.txt`        Post IDs, comment authors, and comment text
`notifications.txt`   Notification recipients and messages
Keep these files in the same folder from which you run the application
if you want existing data to load correctly. The application writes
updated data when it saves, such as on logout or exit. Avoid manually
changing the file contents unless you understand the format.
Project Constraints
Implemented in C++.
Uses classes, pointers, dynamic memory allocation, and custom data
structures.
Does not use STL containers such as `vector`, `list`, `map`, or
`queue`.
Uses a menu-driven console interface.
Uses text-file storage for persistence.
Notes and Limitations
This is an educational DSA project and a simplified social network
simulation, not a production-ready social media platform.
Passwords are stored as plain text in the local `users.txt` file.
This is not suitable for real applications.
Data files should be kept with the executable's working directory to
preserve access to saved information.
The program is designed for a single local run environment and does
not provide a networked or multi-device service.
Authors
Mohadissa Zehra (25K-0663) | Alishba Fatima Rana (25K-0803)
Course: Data Structures and Algorithms  
Language: C++  
Application: Console-based Social Network System
