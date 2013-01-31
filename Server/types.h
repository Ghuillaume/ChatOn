

struct connected_people {
    char ip[15];
    char pseudo[31];
    int time_iddle;
};

struct message {
	connected_people source;
	connected_people dest;
	char message[500];
}

