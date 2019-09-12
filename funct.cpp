#include "funct.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <fstream>
#include <stdio.h>
#include <string.h>

//Units method
/*
*Methode qui permet le mouvement des unités à travers le
*plateau passé en argument
*/
void Units::move(Units* board[12]){
	Units temp;
	switch(m_ownerPlayer){
		case BASE_A:
			if(m_pos < 10){
				board[m_pos+1] = board[m_pos];
				board[m_pos] = &temp;
				board[m_pos]->setUnitType(EMPTY);
				m_pos++;
				break;
			}
			else{
                break;
			}
		case BASE_B:
			if(m_pos > 1){
				board[m_pos-1] = board[m_pos];
				board[m_pos] = &temp;
				board[m_pos]->setUnitType(EMPTY);
				m_pos--;
				break;
			}
			else{
                break;
			}
		default:
			break;
	}
	
}

/*
*Fonction qui renvoie la position de l'ennemi le plus proche sur le
*plateau donné en argument
*/
int Units::getClosestEnnemyInReach(Units* board[12]){
	int directionOfEvolution = (m_ownerPlayer == BASE_A) ? 1:-1;
	int observedPos;
	for(int i = 1; i <= m_reach; i++){
		observedPos = m_pos + directionOfEvolution*i;
		if(observedPos > 11 || observedPos < 0){
			return -2;
        }
		if((!board[observedPos]->isFree() && 
			board[observedPos]->getOwner() != m_ownerPlayer)||((observedPos == 0 || observedPos == 11) && board[observedPos]->isFree())){
			return observedPos;
		}
	} 
	return -3;
}

/*
*Fonction qui permet d'effectuer l'action 1, attaquer l'unité
*située à la positin posToAttack
*Renvoie true si l'attaque a éliminé l'unité et false sinon
*/
bool Units::action1(Units* board[12], int posToAttack){
	std::string unitType;
	int dmgToTake = this->getATK();
	board[posToAttack]->takeDmg(dmgToTake);
	//on détermine la position à attaquer
	this->setHasPlayedAction1InTurn(true);
    if(board[posToAttack]->getHP() <= 0){
		Units temp(EMPTY);
		board[posToAttack] = &temp;
        return true;
	}
	//génération du message 
	switch(m_unitType){
		case INFANTRY:
			unitType = "infantry";
			break;
		case ARCHER:
			unitType = "archer";
			break;
		case CATAPULT:
			unitType = "catapult";
			break;
		case SUPERSOLDIER:
			unitType = "super soldier";
			break;
	}
	std::string attackedUnit;
	if(this->getPos() >= 10)
		attackedUnit = "U"+std::to_string(this->getPos());
	else
		attackedUnit = "U0"+std::to_string(this->getPos());
	std::cout<<"The "<<unitType<<" at "<<this->getPos()<<" of player "<<this->getOwner()<<" is attacking base "<<attackedUnit<<std::endl;
    return false;
}

/*
*Méthode qui permet d'effectuer l'action 2, bouger l'unité sur le
*plateau board
*/
void Units::action2(Units* board[12]){
	std::cerr<<""<<std::endl;
	std::string unitType;
	int directionOfEvolution = (m_ownerPlayer == BASE_A) ? 1:-1;
	//détermination de la direction d'évolution
	if(!this->getHasPlayedAction2InTurn() && 
		(board[m_pos+directionOfEvolution]->isFree())){
		//si l'unité vers laquelle on bouge est vide on avance
		this->move(board);
		this->setHasPlayedAction2InTurn(true);
		switch(m_unitType){
			case INFANTRY:
				unitType = "infantry";
				break;
			case ARCHER:
				unitType = "archer";
				break;
			case CATAPULT:
				unitType = "catapult";
				break;
			case SUPERSOLDIER:
				unitType = "super soldier";
				break;
		}
		//génération du message 
		std::cout<<"The "<<unitType<<" at "<<this->getPos()<<" of player "<<this->getOwner()<<" is moving to "<<m_pos+directionOfEvolution<<std::endl;
	}
}

/*
*Fonction qui permet d'effectuer l'action 3. La fonction est seulement
*initialisé dans Units
*/
bool Units::action3(Units* board[12], int posToAttack){
	(void) board;
	(void) posToAttack;
	return false;
}

/*
*Fonction qui renvoie si l'unité sur le plateau est vide
*/
bool Units::isFree(){
    return (m_unitType <= EMPTY || m_unitType > 4);
}

void Units::takeDmg(int dmg){
	this->m_hp -= dmg;
}


//Infantry method
/*
*Méthode qui fait d'un fantassin un super-soldat
*/
void Infantry::promote(){
	m_promotedToSuperSoldier = true;
	m_unitType = SUPERSOLDIER;
	std::cout<<"The infantry at "<<this->getPos()<<" of player "<<this->getOwner()<<" is getting promoted!"<<std::endl; 
}

/*
*Fonction qui permet d'effectuer l'action 1 pour un fantassin,
*attaquer l'unité située à la positin posToAttack
*Renvoie true si l'attaque a éliminé l'unité et false sinon
*/
bool Infantry::action1(Units* board[12], int posToAttack){
	bool attackingInfantry = ((board[posToAttack]->getUnitType() == INFANTRY) || (board[posToAttack]->getUnitType() == SUPERSOLDIER));
    bool killed = Units::action1(board, posToAttack);
    if(killed && attackingInfantry) //si l'unité à été éliminée le fantassi est promu
        this->promote();
    return killed;
}

/*
*Méthode qui permet d'effectuer l'action 2 pour le fantassin
*/
void Infantry::action2(Units* board[12]){
	this->Units::action2(board);
}

/*
*Fonction qui permet d'effectuer l'action 3 pour un fantassin
*Renvoie true si l'attaque a éliminé l'unité et false sinon
*/
bool Infantry::action3(Units* board[12], int posToAttack){
	if(m_promotedToSuperSoldier){ 
		//si le fantassin est un super-soldat il attaque à nouveau
		return this->action1(board, posToAttack);
	}
	else{
		//si le fantassin n'est pas un super-soldat...
		if(!m_hasPlayedAction1InTurn) //...le fantassin attaque s'il n'a pas déja attaqué
			return this->action1(board, posToAttack);
	}
	return false;
}

//Archer method
/*
*Fonction qui permet d'effectuer l'action 1 pour un archer,
*attaquer l'unité située à la positin posToAttack
*Renvoie true si l'attaque a éliminé l'unité et false sinon
*/
bool Archer::action1(Units* board[12], int posToAttack){
    return Units::action1(board, posToAttack);
}

/*
*Méthode qui permet d'effectuer l'action 2 pour un archer
*/
void Archer::action2(Units* board[12]){
	this->Units::action2(board);
}

//Catapult method
/*
*Fonction qui permet d'effectuer l'action 1 pour un archer,
*attaquer l'unité située à la positin posToAttack
*Renvoie true si l'attaque a éliminé l'unité et false sinon
*/
bool Catapult::action1(Units* board[12], int posToAttack){
	int directionOfEvolution = (m_ownerPlayer == BASE_A) ? 1:-1;
	int distanceToTarget1 = this->getPos() + directionOfEvolution*posToAttack; //détermination de la direction d'évolution
	if(distanceToTarget1 == 4){
		//si l'ennemi le plus proche est située à une distance de 4
		//on attaque cette case et la case juste avant
		bool firstShot = Units::action1(board, posToAttack);
		bool scdShot = Units::action1(board, posToAttack-directionOfEvolution);
		return (firstShot || scdShot);
	}
    else{
		//sinon l'ennemi le plus proche est une distance entre 2 et 3
		//on attaque cette case et la case juste après
		bool firstShot = Units::action1(board, posToAttack);
		bool scdShot = Units::action1(board, posToAttack+directionOfEvolution);
		return (firstShot || scdShot);
	}
}

/*
*Méthode qui permet d'effectuer l'action 2 pour une catapulte
*/
void Catapult::action2(Units* board[12]){
	(void) board;
	this->setHasPlayedAction2InTurn(m_hasPlayedAction1InTurn);
}

/*
*Fonction qui permet d'effectuer l'action 3 pour une catapulte,
*attaquer l'unité située à la positin posToAttack
*Renvoie false dans toutes les situations, ce n'est pas une attaque
*/
bool Catapult::action3(Units* board[12], int posToAttack){
	(void) posToAttack;
	if(!m_hasPlayedAction1InTurn){
		//si la catapulte n'a pa joué la première action, elle avance
		this->Units::action2(board);
		return false;
	}
	return false;
}

//Player methods
/*
*Méthode qui permet l'ajout de l'argent au début du tour
*/
void Player::addTurnMoney(){
	this->m_money += 8;
}

/*
*Méthode qui remet à false les booléens de vérification de tour des
*unités
*/
void Player::setTurnBoolFalseForAll(){
	for(int i=0; i<m_nbOfUnits; i++){
		m_army[i]->setHasPlayedAction1InTurn(false);
		m_army[i]->setHasPlayedAction2InTurn(false);
	}
}

/*
*Méthode qui permet l'ajout d'une unité à l'armée du joueur
*Le type d'unité est donné par le paramètre unitCode
*/
void Player::addUnit(int unitCode){
	Infantry* newUnitI = new Infantry(m_base, m_base*11);
	Archer* newUnitA = new Archer(m_base, m_base*11);
	Catapult* newUnitC  = new Catapult(m_base, m_base*11);
	std::string unitType;
    switch(unitCode) 
	{
		//switch qui ajoute l'unité
		//on ajoute si le joueur a assez d'argent
        case INFANTRY:
            if(this->m_money >= newUnitI->getPrice()){
                m_nbOfUnits++;
                m_army[m_nbOfUnits - 1] = newUnitI;
				unitType = "n infantry";
            }
			else{
				std::cout<<"Not enough money to buy this unit"<<std::endl;
				return;
			}
            break;
        case ARCHER:
            if(this->m_money >= newUnitA->getPrice()){
                m_nbOfUnits++;
                m_army[m_nbOfUnits - 1] = newUnitA;
				unitType = "n archer";
            }
			else{
				std::cout<<"Not enough money to buy this unit"<<std::endl;
				return;
			}
            break;
        case CATAPULT:
            if(this->m_money >= newUnitC->getPrice()){
                m_nbOfUnits++;
                m_army[m_nbOfUnits - 1] = newUnitC;
				unitType = " catapult";
            }
			else{
				std::cout<<"Not enough money to buy this unit"<<std::endl;
				return;
			}
            break;
    }
	//génération du message 
	std::cout<<"Player "<<m_base<<" is adding a"<<unitType<<std::endl;
}

/*
*Méthode qui permet de lancer la résolution des actions 1
*des unités du joueur
*/
void Player::unitAction1(Units* board[12], Player &ennemy){
	std::cout<<"Resolution of the unit action 1 phase  for player on base "<<m_base<<std::endl;
	int posToAttack;
	int dmgToTake;
	for(int i=m_nbOfUnits-1; i >= 0; i--){
		//les unités sont parcourus dans le sens inverse de leur création
		//ceci permettra de parcourir les unités à partir de la plus proche de la base
		Units* temp = m_army[i];
		posToAttack = temp->getClosestEnnemyInReach(board);
		//on détermine la position que l'unité va attaquer
		if(posToAttack >=0){//si on peut attaquer une unité
			if(!temp->getHasPlayedAction1InTurn()){
				dmgToTake = temp->getATK();
				if(posToAttack == 0 && (board[0]->getUnitType() <= EMPTY || board[0]->getUnitType() > 4)){
					//on vérifie si l'on veut attaquer une base
					ennemy.takeDmg(dmgToTake);
					temp->setHasPlayedAction1InTurn(true);
				}
				else if(posToAttack == 11 && (board[11]->getUnitType() <= EMPTY || board[11]->getUnitType() > 4)){
					//on vérifie si l'on veut attaquer une base
					ennemy.takeDmg(dmgToTake);
					temp->setHasPlayedAction1InTurn(true);
				}
				else{
					//on attaque les autres unités
					int dmgToTake = temp->getATK();
					board[posToAttack]->takeDmg(dmgToTake);

					if(board[posToAttack]->getHP() <= 0){
						Units temp(EMPTY);
						board[posToAttack] = &temp;
						ennemy.refreshArmy();
					}
					temp->setHasPlayedAction1InTurn(true);
                    int moneyToAdd = board[posToAttack]->getPrice() / 2;
                    bool killed = temp->action1(board, posToAttack);
                    if(killed)
                        m_money += moneyToAdd;
                        ennemy.refreshArmy();
				}
			}
		}
	}
}

/*
*Méthode qui permet de lancer la résolution des actions 2
*des unités du joueur
*/
void Player::unitAction2(Units* board[12]){
	std::cout<<"Resolution of the unit action 2 phase  for player on base "<<m_base<<std::endl;
	for(int i = 0; i < this->m_nbOfUnits; i++){
		//on parcourt les unités dans leur ordre de création
		m_army[i]->action2(board);
	}
}

/*
*Méthode qui permet de lancer la résolution des actions 3
*des unités du joueur
*/
void Player::unitAction3(Units* board[12], Player &ennemy){
	std::cout<<"Resolution of the unit action 3 phase for player on base "<<m_base<<std::endl;
	int posToAttack;
	int dmgToTake;
	for(int i = 0; i < this->m_nbOfUnits; i++){
		//on parcourt les unités dans leur ordre de création
		Units* temp = m_army[i];
		posToAttack = temp->getClosestEnnemyInReach(board);
		//vérification des cas particuliers
		if(posToAttack >= 0 || temp->getUnitType() == CATAPULT){
			if(!temp->getHasPlayedAction1InTurn() || temp->getUnitType() == SUPERSOLDIER){
				dmgToTake = temp->getATK();
				if(posToAttack == 0 && board[0]->isFree()){
					ennemy.takeDmg(dmgToTake);
					temp->setHasPlayedAction1InTurn(true);
				}
				else if(posToAttack == 11 && board[11]->isFree()){
					ennemy.takeDmg(dmgToTake);
					temp->setHasPlayedAction1InTurn(true);
				}
				else{
                    bool killed = temp->action3(board, posToAttack);
                    if(killed)
                        ennemy.refreshArmy();
				}
			}
		}
	}
}

/*
*Fonction pour extraire les donnéespour créer une sauvegarde
*Renvoie la ligne qui représente les données du joueur
*/
std::string Player::getData(){
    std::cerr<<"In getData"<<std::endl;
    std::string rslt;
    rslt = rslt + std::to_string(m_hp) + " " + std::to_string(m_money) + " ";
    std::string toAdd;
    for(int i=0; i < m_nbOfUnits; i++){
        std::cerr<<"In getData in for "<< i <<std::endl;
        toAdd = toAdd + std::to_string(m_army[i]->getUnitType()) + " ";
        toAdd = toAdd + std::to_string(m_army[i]->getHP()) + " ";
        toAdd = toAdd + std::to_string(m_army[i]->getPos()) + " ";
    }
    rslt = rslt + toAdd;
    return rslt;
}

/*
*Méthode pour lancer les actions des unités du joueur
*/
void Player::unitTurn(Units* board[12], Player &ennemy){
	this->unitAction1(board, ennemy);
	this->unitAction2(board);
	this->unitAction3(board, ennemy);
}

/*
*Méthode qui lance une sauvegarde du jeu
*/
void Player::saveGame(Game& game,std::string fileName){
    std::string gameType = game.getGameType(); //extraction du type du jeu
    std::string turnNumber = std::to_string(game.getCurrentTurnNumber()); // extraction du numéro du tour
    std::string player1State;
    std::string player2State;

	//extraction des données des joueurs
    if(m_base == 0){
        std::cerr<<"First player saving game"<<std::endl;
        player1State = this->getData();
        Player ennemy = game.m_player2;
        player2State = ennemy.getData();
    }
    else{
        player2State = this->getData();
        Player ennemy = game.m_player1;
        player1State = ennemy.getData();
    }
	//enregistrement du fichier de sauvegarde
    fileName +=".txt";
    std::ofstream saveFile;
    saveFile.open (fileName);
    saveFile<<gameType; saveFile<<std::endl;
    saveFile<<turnNumber; saveFile<<std::endl;
    saveFile<<player1State; saveFile<<std::endl;
    saveFile<<player2State; saveFile<<std::endl;
	this->setHP(0);
}

/*
*Méthode qui permet de lancer le tour du joueur pour prendre une décision
*concernant la création d'une nouvelle unité
*/
void Player::playerTurn(Game& game){
    bool validAnswer = false;
    int unitType = -1;
    while(!validAnswer){
       	std::string answer;
	    std::cout<<"Player on base "<<this->getBase()<<", your orders \"general\"?"<<std::endl;
        std::cin>>answer;
        std::string instr = answer.substr(0,1);
        std::string type = answer.substr(1,2);
        
        if(instr == "a"){
			//le joueur veut ajouter une unité
			//la deuxième lettre donnera le type d'unité
            if(type == "I" || type == "i"){
                type = "infantry";
                unitType = INFANTRY;
                validAnswer = true;
            }
            else if(type == "A" || type == "a"){
                type = "archer";
                unitType = ARCHER;
                validAnswer = true;
            }
            else if(type == "C" || type == "c"){
                type = "catapult";
                unitType = CATAPULT;
                validAnswer = true;
            }
/*
            if(unitType != -1){
                std::cout<<"Player on base "<<this->getBase()<<", is adding "<< type << " unit."<<std::endl;
            }
*/
        }
		else if(instr == "n") //le joueur ne veut pas ajouter d'unité
			validAnswer = true;
		else if(instr == "q"){
			//le joueur veut abandonner
			this->setHP(0);
			validAnswer = true;
		}
		else if(instr == "s"){
			//le joueur veut sauvegarder
			std::string fileName;
			std::cout<<"Name of the game data file?"<<std::endl;
			std::cin>>fileName;
			saveGame(game, fileName);
			validAnswer = true;
		}
        if(!validAnswer) //la réponse n'est pas valide
            std::cout<<"Unclear instructions, general! Please choose again..."<<std::endl;
    }
    this->addUnit(unitType);
}

/*
*Méthode qui permet de mettre à jour l'armée en enlevant les unités
*éliminées
*/
void Player::refreshArmy(){
	Units** newArmy = new Units*[m_nbOfUnits];
	int currentPos = 0;
	int currentNbUnits = m_nbOfUnits;
	for(int i = 0; i<currentNbUnits; i++){
		if(m_army[i]->getHP() > 0){
			newArmy[currentPos] = m_army[i];
			currentPos++;
		}
		else{
			m_nbOfUnits--;
		}
	}
	delete [] m_army;
	this->setArmy(newArmy);
}

/*
*Permet de lancer la décision de l'IA
*L'IA est basique et elle décide d'ajouter l'unité la plus chère qu'elle
*peut acheter
*/
void Player::AITurn(){
	int currentMoney = this->getMoney();
	if(currentMoney >= 20){
		std::cerr<<"The AI is adding a catapult"<<std::endl;
		this->addUnit(CATAPULT);
	}
	else if(currentMoney >= 12){
		std::cerr<<"The AI is adding an archer"<<std::endl;
		this->addUnit(ARCHER);
	}
	else if(currentMoney >= 10){
		std::cerr<<"The AI is adding an infantry"<<std::endl;
		this->addUnit(INFANTRY);
	} 
}

/*
*Méthode qui permet d'ajouter les données du joueur stocker dans 
*le vecteur playerDetails
*/
void Player::loadPlayerData(std::vector<int> playerDetails){
	m_hp = playerDetails[0];
	m_money = playerDetails[1];
	int nbOfUnits = (playerDetails.size() - 2)/3;
	std::cerr<<"Player has "<<m_nbOfUnits<<" units"<<std::endl;
	for(int i = 0; i<nbOfUnits; i = i+1){
		std::cerr<<"Considering unit number "<<i<<std::endl;
		int unitCode = playerDetails[i*3+2];
		if(unitCode == SUPERSOLDIER)
			unitCode = INFANTRY;
		this->addUnit(unitCode);
		int hp = playerDetails[i*3+3];
		int pos = playerDetails[i*3+4];
		m_army[i]->setHP(hp);
		m_army[i]->setPos(pos);
		if(playerDetails[i*3+2] == SUPERSOLDIER)
			m_army[i]->setUnitType(SUPERSOLDIER);
	}
}

//Game method
/*
*Fonction qui permet de savoir si une case est vide
*/
bool Game::squareFree(int squareNb){
    int temp = m_board[squareNb]->getUnitType();
    if(temp <= 0 || temp>4)
        return true;
    else
        return false;
}

/*
*Méthode qui permet de savoir si la partie est terminée
*/
void Game::verifyGameEnding(){
	if(this->m_turnNumber > this->m_maxTurnNumber){
		this->m_gameFinished = true;
		std::cout<<"Too many rounds! "<<m_gameFinished<<std::endl;
	}
	else if(this->m_player1.getHP() <= 0 || this->m_player2.getHP() <= 0){
		this->m_gameFinished = true;
		std::cout<<"Player died!"<<std::endl;
	}
    else
        this->m_gameFinished = false;
}

/*
*Méthode qui effectue les actions nécessaire pour commencer un tour
*/
void Game::startTurn(){
	m_player1.addTurnMoney();
	m_player1.setTurnBoolFalseForAll();
	m_player2.addTurnMoney();
	m_player2.setTurnBoolFalseForAll();
}

/*
*Méthode qui permet d'actualiser le plateau de jeu
*/
void Game::refreshBtlground(){
    int squareToRefresh;
	int refreshedSquares[12];
	for(int i = 0; i <12; i++){
		refreshedSquares[i] = 0;
	}
    
	for(int i = 0; i < m_player1.getNbOfUnits(); i++){
		//on acutualise les cases occupées par l'armée du joueur à la base 0
	    squareToRefresh = m_player1.getArmy()[i]->getPos();
	    m_board[squareToRefresh] = m_player1.getArmy()[i];
		refreshedSquares[squareToRefresh] = 1;
	}

	for(int i = 0; i < m_player2.getNbOfUnits(); i++){
		//on acutualise les cases occupées par l'armée du joueur à la base 1
	    squareToRefresh = m_player2.getArmy()[i]->getPos();
	    m_board[squareToRefresh] = m_player2.getArmy()[i];
		refreshedSquares[squareToRefresh] = 1;
	}

	for(int i = 0; i<12; i++){
		//on acutualise les cases vides
		if(refreshedSquares[i] == 0){
			Units temp;
			std::cerr<<"Creating temp"<<std::endl;
            temp.setUnitType(EMPTY);
			std::cerr<<"Emptying temp"<<std::endl;
            m_board[i] = &temp;
			std::cerr<<"Affecting temp"<<std::endl;
			refreshedSquares[i] = 1;
		}
	}
}

/*
*Fonction qui permet de lancer un tour de jeu
*On renvoie la valeur du booléen qui permet la vérification de la fin du jeu
*/
bool Game::gameTurn(){
	this->startTurn();//on effectue les actions de départ d'un tour
	//this->refreshBtlground();
	/*on suit le même schéma pour les deux joueurs, si le joeur possède une unité, le actions des unités sont lancées. Si la case de la base du joeur est libre on lui demande s'il veut ajouter une unité*/
    if(m_player1.getNbOfUnits() > 0){
		this->print(false);
        m_player1.unitTurn(this->m_board, m_player2);
		this->print(false);
		verifyGameEnding();
		this->refreshBtlground();
	}
    if(this->squareFree(0) && m_player1.getMoney() >= 8){
		this->print(true);
	    m_player1.playerTurn(*this);
		this->refreshBtlground();
		this->print(false);
		verifyGameEnding();
	}
    if(m_player2.getNbOfUnits() > 0){
		this->print(false);
        m_player2.unitTurn(this->m_board, m_player1);
		this->print(false);
		verifyGameEnding();
		this->refreshBtlground();
	}
    if(this->squareFree(11) && m_player2.getMoney() >= 8){
		this->print(true);
	    m_player2.playerTurn(*this);
		this->refreshBtlground();
		this->print(false);
	}
	this->m_turnNumber++;
	verifyGameEnding();
	return this->m_gameFinished;
}

/*
*Méthode pour intialiser les données du jeu
*/
void Game::gameInit(){
    Player playerA(0);
    Player playerB(1);
    m_player1 = playerA;
    m_player2 = playerB;
	m_turnNumber = 0;
    for (int i = 0; i < 12; i++)
        m_board[i] = new Units();
	this->print(false);
}

/*
*Méthode pour afficher l'état du plateau
*L'argument commandeAff permettra l'affichage des commandes s'il vaut true
*/
void Game::print(bool commandAff){
	std::string stringHP  = "";
	std::string stringType = "";
	std::string last = "B00 U01 U02 U03 U04 U05 U06 U07 U08 U09 U10 B01";
	std::string command = "Instructions for dumb players...SORRY SORRY NEW, new player\nTo create a unit:\n\tInfantry: ai\n\tArcher: aa\n\tCatapult: ac\nTo skip your turn: n\nTo give up (you, coward!): q\nTo save game and exit: s";
	std::cout<<std::endl;
	std::cout<<"Turn number "<<this->m_turnNumber<<"----------------------------------"<<std::endl;
	std::cout<<std::endl;
	std::cout<<"Player 1 HP: "<< this->m_player1.getHP()<<std::endl;
    std::cout<<"     Money : "<< this->m_player1.getMoney()<<std::endl;
	std::cout<<"Player 2 HP: "<< this->m_player2.getHP()<<std::endl;
    std::cout<<"     Money : "<< this->m_player2.getMoney()<<std::endl;
	std::cout<<std::endl;
	for(int i = 0;  i < 12; i++){
        int temp = m_board[i]->getUnitType();
		int HP = m_board[i]->getHP();
		std::string space ="";
		if(HP == 10)
			space = " ";
		else
			space = "__";
		switch(temp){
			case INFANTRY:
				stringHP = stringHP + std::to_string(HP) + space + " ";
				stringType = stringType + std::to_string(this->m_board[i]->getOwner()) + "#I ";
				break;
			case ARCHER:
				stringHP = stringHP + std::to_string(HP) + space + " ";
				stringType = stringType + std::to_string(this->m_board[i]->getOwner()) + "#A ";
				break;
			case CATAPULT:
				stringHP = stringHP + std::to_string(HP) + space + " ";
				stringType = stringType + std::to_string(this->m_board[i]->getOwner()) + "#C ";
				break;
			case SUPERSOLDIER:
				stringHP = stringHP + std::to_string(HP) + space  + " ";
				stringType = stringType + std::to_string(this->m_board[i]->getOwner()) + "#S ";
				break;
			case EMPTY:
				stringHP = stringHP + "___ ";
				stringType = stringType + "### ";
				break;
			default:
				stringHP = stringHP + "___ ";
				stringType = stringType + "### ";
				break;
		}
	}
	std::cout<<stringHP<<std::endl;
	std::cout<<stringType<<std::endl;
	std::cout<<last<<std::endl;
	if(commandAff)
		std::cout<<command<<std::endl;
	std::cout<<"Done reading? Press enter to continue..."<<std::endl;
	getchar();
}

/*
*Méthode qui réunit l'initialisation et le lancement de la partie
*/
void Game::startGame(){
	this->gameInit();
    this->play();
}

/*
*Méthode pour le lancement d'une partie
*/
void Game::play(){
	bool endGame = false;
    while(!endGame){
		//tant que le jeu n'a pas pris fin on continue de lancer un
		//nouveau tour
        std::cout<<"Turn number "<< this->m_turnNumber<<std::endl;
        std::cout<<"Moving to the next turn!"<<std::endl;
		endGame = this->gameTurn();
    }
    std::cout<<"Game Over! Finally...leave"<<std::endl;
}

/*
*Fonction qui permet de charger des données sauvegardées
*On renvoie true si le chargement n'a pas posé de problèmes et false sinon
*/
bool Game::loadData(std::string answer){
	std::cerr<<"In load data"<<std::endl;
	std::string line;
	char * cline = new char[81];
	std::vector<int> player1details;
	std::vector<int> player2details;
	Player playerA(BASE_A);
	Player playerB(BASE_B);
	char *data;
	std::ifstream dataFile (answer);
	if(dataFile.is_open()){
		getline(dataFile, line);//ignoring first line
		getline(dataFile, line);//line with turn number
		strcpy(cline, line.c_str());
		data = strtok(cline, " ");
		m_turnNumber = atoi(data);
		getline(dataFile, line);//line with player 1 details
		strcpy(cline, line.c_str());
		data = strtok(cline, " ");
		while(data != NULL){
			player1details.push_back(atoi(data));
			std::cerr<<"Adding into player 1 data vector "<<data<<std::endl;
			data = strtok (NULL, " ");
		}
		if(player1details.size()%3 != 2){
			return false;//data is missing
		}
		getline(dataFile, line);//line with player 2 details
		strcpy(cline, line.c_str());
		data = strtok(cline, " ");
		while(data != NULL){
			std::cerr<<"Adding into player 2 data vector "<<data<<std::endl;
			player2details.push_back(atoi(data));
			data = strtok (NULL, " ");
		}
		if(player2details.size()%3 != 2){
			return false;//data is missing
		}
		delete [] cline;
		playerA.loadPlayerData(player1details);
		playerB.loadPlayerData(player2details);
		m_player1 = playerA;
		m_player2 = playerB;
		this->refreshBtlground();
		return true;
	}
	else
		return false;
}

//GameAI methods
/*
*Fonction similaire à celle de Game pour un jeu contre IA
*/
bool GameAI::gameTurn(){
	this->startTurn();
	//this->refreshBtlground();
    if(m_player1.getNbOfUnits() > 0){
		this->print(false);
        m_player1.unitTurn(this->m_board, m_player2);
		this->print(false);
		verifyGameEnding();
	}
	this->refreshBtlground();
    if(this->squareFree(0)){
		this->print(true);
		//Game& tempGame = this;
	    m_player1.playerTurn(*this);
		this->refreshBtlground();
		this->print(false);
	}
    if(m_player2.getNbOfUnits() > 0){
		this->print(false);
        m_player2.unitTurn(this->m_board, m_player1);
		this->print(false);
		verifyGameEnding();
	}
	this->refreshBtlground();
    if(this->squareFree(11)){
		this->print(false);
	    m_player2.AITurn();
		this->refreshBtlground();
		this->print(false);
	}
	this->m_turnNumber++;
	verifyGameEnding();
	return this->m_gameFinished;
}

/*
*Méthode d'affichage pour une partie contre IA
*/
void GameAI::print(bool commandAff){
	std::string stringHP  = "";
	std::string stringType = "";
	std::string last = "B00 U01 U02 U03 U04 U05 U06 U07 U08 U09 U10 B01";
	std::string command = "Instructions for dumb players...SORRY SORRY NEW, new player\nTo create a unit:\n\tInfantry: ai\n\tArcher: aa\n\tCatapult: ac\nTo skip your turn: n\nTo give up (you, coward!): q\nTo save game and exit: s";
	std::cout<<std::endl;
	std::cout<<"Turn number "<<this->m_turnNumber<<"----------------------------------"<<std::endl;
	std::cout<<std::endl;
	std::cout<<"Player 1 HP: "<< this->m_player1.getHP()<<std::endl;
    std::cout<<"     Money : "<< this->m_player1.getMoney()<<std::endl;
	std::cout<<"Player 2 HP: "<< this->m_player2.getHP()<<std::endl;
    std::cout<<"     Money : "<< this->m_player2.getMoney()<<std::endl;
	std::cout<<std::endl;
	for(int i = 0;  i < 12; i++){
        int temp = m_board[i]->getUnitType();
		int HP = m_board[i]->getHP();
		std::string space ="";
		if(HP == 10)
			space = " ";
		else
			space = "__";
		switch(temp){
			case INFANTRY:
				stringHP = stringHP + std::to_string(HP) + space + " ";
				stringType = stringType + std::to_string(this->m_board[i]->getOwner()) + "#I ";
				break;
			case ARCHER:
				stringHP = stringHP + std::to_string(HP) + space + " ";
				stringType = stringType + std::to_string(this->m_board[i]->getOwner()) + "#A ";
				break;
			case CATAPULT:
				stringHP = stringHP + std::to_string(HP) + space + " ";
				stringType = stringType + std::to_string(this->m_board[i]->getOwner()) + "#C ";
				break;
			case SUPERSOLDIER:
				stringHP = stringHP + std::to_string(HP) + space  + " ";
				stringType = stringType + std::to_string(this->m_board[i]->getOwner()) + "#S ";
				break;
			case EMPTY:
				stringHP = stringHP + "___ ";
				stringType = stringType + "### ";
				break;
			default:
				stringHP = stringHP + "___ ";
				stringType = stringType + "### ";
				break;
		}
	}
	std::cout<<stringHP<<std::endl;
	std::cout<<stringType<<std::endl;
	std::cout<<last<<std::endl;
	if(commandAff){
		std::cout<<command<<std::endl;
		std::cout<<"Done reading?";
	}
	std::cout<<"Press enter to continue..."<<std::endl;
	getchar();
}

/*
*Fonction similaire à celle de Game pour un jeu contre IA
*/
void GameAI::startGame(){
	this->gameInit();
    this->play();
}

/*
*Fonction similaire à celle de Game pour un jeu contre IA
*/
void GameAI::play(){
	bool endGame = false;
	while(!endGame){  
        std::cout<<"Turn number "<< this->m_turnNumber<<std::endl;
        std::cout<<"Moving to the next turn!"<<std::endl;
		endGame = this->gameTurn();
    }
    std::cout<<"Game Over! Finally...leave"<<std::endl;
}


