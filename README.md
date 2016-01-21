# Natural_Language_Processing
Natural Languague Processing in GPU for High Performance Computing

This module aims to do sentimental analysis on twitter message whether it signifies a positive attitude or a negative attitude or a neutral attitude. I have implement this in two methods
- Using Naïve Bayes as described in the paper Twitter sentiment classification using CPU to train and classify data and GPU to parse the raw data into messages
- Using GPU to classify the twitter message based on a huge set of positive and negative words where the messages will look for any word match in the positive and the negative word and depending on the max number of attraction it has, the probability is defined whether it is a positive message or a negative message.

In the application
- The file is read and the data is parsed and stored as vectors in a custom defined structure
- For parsing the data, a 1-D char array and 1-D int array is constructed
- In the char array a record will occupy a space of 300 bytes.
- Twitter messages won’t go longer than 140. The data is processed to first segregate the message in the csv into id, class, sender, message etc.
- The 1-D array will note the indexes of the starting point for each word. This 1-D array will be spaced into 70 integer units for a record assuming that, a twitter message will not have more than 70 words.
- To train the data, the train data words are matched with the positive and negative words to find positive probability and negative probability of each word o During testing, each word is matched with the twitter words and if the word is a match, the probability is multiplied for the respective category.
- If no match a small probability is assigned in each category.

In the GPU version, the words are matched in GPU
- A single thread will be allocated to process a record
- The positive and negative words will be char arrays spaced by 25 bytes for a word
- A twitter message is converted into words by removing all special characters and each word in matched with the entire positive and negative data set
- When a positive match occurs, the probability is incremented and when a negative match occurs the probability is decremented
- The probabilities are stored in 1-D array

In the CPU
- Depending on the probability, they are given whether it is a positive tweet or negative tweet or a neutral tweet.

Block Size
- Uses a standard block size of 32*32
