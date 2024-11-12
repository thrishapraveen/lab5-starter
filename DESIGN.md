1. Each Chat struct contains 2 uint32 variables and 4 pointers.
2 uint32 variables = 2(4 bytes) = 8 bytes
4 pointers = 4(8 bytes on a 64-bit system) = 32 bytes

For 10 chats = 320 bytes
For 100 chats = 3,200 bytes
For 1000 chats = 32,000 bytes

The additional working memory that comes from using /chats request in my code is dependent on the number of chats since /chats has to go through all the chats and print representations of all of the chats each time /chats is called.

To minimize chat storage, I can change the data types of the uint32 variables to be smaller. Since the maximum amount of reactions for a message is 100, I could change the size of the num_reactions variable to be a uint8 variable.

Another way to minimize chat storage would be to use fixed-length arrays for the username and message fields of a chat. Since users can't change their usernames and messages through this simple server, there's no real benefit to having them be pointer variables. 

2. If the container storing all of the chats on the server could be more than 100,000, then users could have more freedom in how often they could post and how many users could post onto the server. However, all_chats would start overwriting other data in memory, which is not secure. I could change my implementation by making all_chats dynamically allocated using pointers.
