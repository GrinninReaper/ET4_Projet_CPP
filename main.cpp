#include "funct.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <stdio.h>
#include <string.h>

int main(){
/*
	std::string file = "100 2 3";
	char *cfile = new char[file.length() + 1];
	strcpy(cfile, file.c_str());
	char *data;
	data = strtok(cfile, " ");
	while(data != NULL){
		int idata = atoi(data);
		std::cout<<idata<<std::endl;
		data = strtok (NULL, " ");
	}
	abort();
*/
	
	
	
	//std::cerr.setstate(std::ios_base::failbit); //permet de suppr
	std::string firstQuote = "Alone, bad. Friend, good.";
	std::string scdQuote = "Beware; for I am fearless and therefore powerful.";

	std::string line;

	std::ifstream rule("_regle.txt");
	if(rule.is_open()){
		while(getline(rule, line))
			std::cout<<line<<std::endl;
		rule.close();
	}
	else 
		std::cout<<"Couldn't open rules file"<<std::endl;


	std::cout<<"So in the mood to kill! =D" << std::endl;
	bool endNotice = true;
	std::string answer;
	while(endNotice){
		std::cout<<"Wanna play? [y/n]?"<<std::endl;
		std::cin>>answer;
        answer = answer.substr(0,1);
		if(answer == "y"){
			std::cout<<"New game or load game? [n/l]"<<std::endl;
			std::cin>>answer;
        	answer = answer.substr(0,1);
			if(answer == "n"){
				std::cout<<"Wanna play with someone? Type s..."<<std::endl;
				std::cout<<"Wanna play with the AI? Type a..."<<std::endl;
				std::cin>>answer;
				answer = answer.substr(0,1);
				if(answer == "s"){
					std::cout<<firstQuote<<std::endl;
		        	Game game;
		        	game.startGame();
				}
				else if(answer == "a"){
					std::cout<<scdQuote<<std::endl;
					GameAI game;
					game.startGame();
				}
			}
			else if(answer == "l"){
                bool fileFound = false;
                while(!fileFound){
                    std::cout<<"Game date file?"<<std::endl;
                    std::cin>>answer;
                    std::ifstream dataFile (answer);
                    if(dataFile.is_open())
                        fileFound = true;
                    std::string line;
                    getline(dataFile, line);
					dataFile.close();
                    if(line == "s"){
                        Game game;
                        bool dataLoaded = game.loadData(answer);
                        if(dataLoaded)
                            game.play();
                        else
                            std::cout<<"File corrupted.\nIn other words, this is not a game data file! I don't read your trash texts!"<<std::endl;
                    }
                    else if(line == "a"){
                        GameAI game;
                        bool dataLoaded = game.loadData(answer);
                        if(dataLoaded)
                            game.play();
                        else
                            std::cout<<"File corrupted.\nIn other words, this is not a game data file! I don't read your trash texts!"<<std::endl;
                    }
                    else{
                        std::cout<<"File corrupted.\nIn other words, this is not a game data file! I don't read your trash texts!"<<std::endl;
                    }
                }
			}
        }
		else{
		    std::cout<<"Leave!" << std::endl;
			endNotice = false;
		}

	}
	std::cout<<std::endl;
	std::ifstream credits("_credits.txt");
	if(credits.is_open()){
		while(getline(credits, line))
			std::cout<<line<<std::endl;
		credits.close();
	}
	else 
		std::cout<<"Couldn't open credits file"<<std::endl;
    return 0;
}
