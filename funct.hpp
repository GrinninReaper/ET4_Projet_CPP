#ifndef FUNCT_HPP
#define FUNCT_HPP

#include <string>
#include <vector>
#include <iostream>

#define BASE_A 0
#define BASE_B 1

#define EMPTY 0
#define INFANTRY 1
#define ARCHER 2
#define CATAPULT 3
#define SUPERSOLDIER 4

/*
class Damageable{
	protected:
		int m_hp;

	public:
		void setHP(int newHP){m_hp = newHP;}
		virtual void takeDmg(int dmg) = 0;
};
*/

class Units/*: public Damageable*/{
	protected:


		int m_ownerPlayer;
		int m_atk;
		int m_reach;
		int m_pos;
		int m_price;
		int m_unitType = EMPTY;
		int m_hp;
		bool m_hasPlayedAction1InTurn;
		bool m_hasPlayedAction2InTurn;
        Units(int owner, int atk, int reach, int pos, int price, int unitType, int hp){
            m_ownerPlayer = owner;
            m_atk = atk;
            m_reach = reach;
            m_pos = pos;
            m_price = price;
            m_unitType = unitType;
            m_hp = hp;
            m_hasPlayedAction1InTurn = false;
            m_hasPlayedAction2InTurn = false;
        }
	public:
		//Constructors, destructor, getters and setters

		Units() :
			m_ownerPlayer(5), m_atk(0), m_reach(0), m_pos(0), m_price(0), m_unitType(EMPTY), m_hp(0), m_hasPlayedAction1InTurn(false), m_hasPlayedAction2InTurn(false)  {}
	
		Units(int unitType):
			m_unitType(unitType) {}

		int getOwner(){ return m_ownerPlayer;}
		int getHP(){ return m_hp;}
		int getATK() { return m_atk;}
		int getReach() { return m_reach;}
		int getPos() { return m_pos;}
		int getPrice() { return m_price;}
		int getUnitType() { return m_unitType;}
		bool getHasPlayedAction1InTurn() { return m_hasPlayedAction1InTurn;}
		bool getHasPlayedAction2InTurn() { return m_hasPlayedAction2InTurn;}

		//setters
		void setHasPlayedAction1InTurn(bool newValue) {m_hasPlayedAction1InTurn = newValue;}
		void setHasPlayedAction2InTurn(bool newValue) {m_hasPlayedAction2InTurn = newValue;}
		void setPos(int newPos) {m_pos = newPos;}
		void setUnitType(int newType) { m_unitType = newType;}
		void setHP(int newHP) {m_hp = newHP;}

		//Methods
        bool isFree();
		void takeDmg(int dmg);

		void move(Units* board[12]);
		int getClosestEnnemyInReach(Units* board[12]);
		virtual bool action1(Units* board[12], int posToAtk);
        virtual void affiche(){std::cerr << "unity" << std::endl;}
		virtual void action2(Units* board[12]);
		virtual bool action3(Units* board[12], int posToAtk);
};

class Infantry:public Units {
	private:
		bool m_promotedToSuperSoldier;
	public:
		//Constructors, destructor, getters and setters
		Infantry(int owner, int pos) : Units(owner, 4, 1, pos, 10, INFANTRY, 10), m_promotedToSuperSoldier(false){}

		Infantry(int ownerPlayer){
			m_ownerPlayer = ownerPlayer;
			m_hp = 10;
			m_atk = 4;
			m_reach = 1;
			m_pos = ownerPlayer * 11;
			m_price = 10;
			m_unitType = INFANTRY;
			m_promotedToSuperSoldier = false;
		}
		~Infantry() {}

		//Methods
        virtual void affiche(){std::cerr << "infantry" << std::endl;}
		void promote();
		/*virtual*/ bool action1(Units* board[12], int posToAtk);
		void action2(Units* board[12]);
		bool action3(Units* board[12], int posToAtk);
		
};

class Archer : public Units {
	private:
	public:
		//Constructors, destructor, getters and setters
		Archer(int owner, int pos): Units(owner, 3, 3, pos, 12, ARCHER, 8) {}
/*
		Archer(int ownerPlayer){
			m_ownerPlayer = ownerPlayer;
			m_hp = 10;
			m_atk = 4;
			m_reach = 3;
			m_pos = ownerPlayer * 11;
			m_price = 10;
			m_unitType = ARCHER;	
		}
*/
		~Archer() {}

		//Methods
        /*virtual*/ bool action1(Units* board[12], int posToAtk);
		void action2(Units* board[12]);
		//bool action3(Units* board[12], int posToAtk);
};

class Catapult : public Units {
	private:
	public:
		//Constructors, destructor, getters and setters
		Catapult(int owner, int pos): Units(owner, 6, 4, pos, 20, CATAPULT, 12) {}
/*
		Catapult(int ownerPlayer){
			m_ownerPlayer = ownerPlayer;
			m_hp = 10;
			m_atk = 4;
			m_reach = 3;
			m_pos = ownerPlayer * 11;
			m_price = 10;
			m_unitType = CATAPULT;
		}
*/
		~Catapult() {}

		//Methods
		int getClosestEnnemyInReach(Units* board[12]);
		/*virtual*/ bool action1(Units* board[12], int posToAtk);
		void action2(Units* board[12]);
		bool action3(Units* board[12], int posToAtk);
};

class Game;

class Player /*: public Damageable*/{
	private:
		int m_base;
		Units** m_army = new Units* [0];
        int m_nbOfUnits = 0;
		int m_hp;
		int m_money;
	public:
		//Constructors, destructor, getters and setters
		Player() {}

		Player(int base){
			m_base = base;
			m_hp = 100;
			m_money = 0;
            //m_army = (Units**) malloc(sizeof(Units*) * m_nbOfUnits);
            m_nbOfUnits = 0;
		}

		~Player() {}

		int getBase(){return m_base;}
		Units** getArmy(){return m_army;}
		int getHP(){return m_hp;}
		int getMoney(){return m_money;}
        int getNbOfUnits(){return m_nbOfUnits;}

		void setHP(int hp) {m_hp = hp;}
		void setMoney(int money) {m_money = money;}
		void setArmy(Units ** army){m_army = army;}

		//Methods
		void takeDmg(int dmg){
			this->m_hp -= dmg;
		}
		void addTurnMoney();
		void setTurnBoolFalseForAll();
		void addUnit(int unitCode);
		void unitAction1(Units* board[12], Player &ennemy);
		void unitAction2(Units* board[12]);
		void unitAction3(Units* board[12], Player &ennemy);
        void unitTurn(Units* board[12], Player &ennemy);
		void saveGame(Game& game,std::string fileName);
		void playerTurn(Game& game);
		void refreshArmy();
		void AITurn();
        std::string getData();
		void loadPlayerData(std::vector<int> playerDetails);
};

class Game{
	protected:
		Player m_player1;
		Player m_player2;
		int m_turnNumber = 0;
		const int m_maxTurnNumber = 100;
        Units** m_board = new Units*[12];
		bool m_gameFinished;
	
	public:
		//Constructors, destructor, getters and setters
		Game() {
			m_turnNumber = 0;
		}
		Game(Player player1, Player player2){
			m_player1 = player1;
			m_player2 = player2;
		}

		~Game() {}

		Player getPlayer1(){ return m_player1; }
		Player getPlayer2(){ return m_player2; }
		int getCurrentTurnNumber(){return m_turnNumber;}

		//Methods
		friend void Player::saveGame(Game& game,std::string fileName);
        bool squareFree(int squareNb);
		void emptySquare(int);
		void verifyGameEnding();
		void startTurn();
		virtual bool gameTurn();
        void refreshBtlground();
        void gameInit();
		virtual void print(bool command);
        virtual void startGame();
        virtual void play();
        virtual std::string getGameType(){ return "s";};
        bool loadData(std::string answer);
};

class GameAI: public Game{
	public:
		bool gameTurn();
		void print(bool command);
        void startGame();
		void play();
        std::string getGameType(){ return "a";};
};

#endif
