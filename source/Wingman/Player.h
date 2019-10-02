#pragma once
#include <SFML\/Audio.hpp>
#include"Bullet.h"
#define START_HP 25
#define MAX_DAME 5
#define ADD_HP 10
#define DT_MUTILPLIER 62.5f
#define DAME_TIMER_MAX 40.f
#define SHOOT_TIMER_MAX 20.f
#define POWERUP_TIMER_MAX 400.f
#define SHOCK_TIME 200
class Player
{
private:
	//CORE
	float dtMultiplier;
	float keyTimeMax;
	float keyTime;

	unsigned playerNr;

	//Positions
	Vector2f playerCenter;

	//Timers
	float shootTimer;
	float shootTimerMax;
	float damageTimer;
	float damageTimerMax;
	
	float powerupTimerMax;
	float powerupTimer;

	Sprite sprite;
	RectangleShape hitBox;

	//Accessories
	Sprite mainGunSprite;
	dArr<Bullet> bullets;


	Sprite lWing;
	Sprite rWing;
	Sprite cPit;
	Sprite aura;

	int SkinSelect;
	

	dArr<int> controls;
	Vector2f currentVelocity;
	float maxVelocity;
	float acceleration;
	Vector2f direction;
	float stabilizerForce;
	Vector2f normDir;

	int level;
	int exp;
	int expNext;
	int statPoints;

	int armor; //endurance
	int plating; //vigor
	int power; //strength
	int agility; //agility

	int hp;
	int hpMax;
	int hpAdded;

	int damage;
	int damageMax;

	int score;

	int currentWeapon;

	//UPGRADES
	dArr<int> upgradesAcquired;
	int mainGunLevel;
	bool piercingShot;
	bool dualMissiles01;
	bool dualMissiles02;

	//Powerups
	bool powerupRF;
	bool powerupXP;

	static sf::SoundBuffer soundfireB;
	static sf::Sound soundFire;
public:
	Player(
		int UP = Keyboard::Up, 
		int DOWN = Keyboard::Down,
		int LEFT = Keyboard::Left, 
		int RIGHT = Keyboard::Right,
		int SHOOT = Keyboard::Space,
		int STATS = Keyboard::Tab
	);
	virtual ~Player();

	//Accessors
	Bullet& getBullet(unsigned index);
	void removeBullet(unsigned index);
	inline const int getBulletsSize()const { return this->bullets.size(); }
	inline Vector2f getPosition()const { return this->sprite.getPosition(); }
	inline void resetVelocity() { this->currentVelocity = Vector2f(0.f, 0.f); }
	inline void move(float x, float y) { this->sprite.move(x, y); this->mainGunSprite.move(x, y); }
	inline const Vector2f& getNormDir()const { return this->normDir; }
	inline FloatRect getBounds()const { return this->sprite.getGlobalBounds(); }
	void setPos(float x, float y);
	
	inline const bool intersects(FloatRect rect)const { return this->sprite.getGlobalBounds().intersects(rect); }

	inline const String getHpAsString()const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax); }
	inline const String getExpAsString()const { 
		if (this->expNext > 0)
			return std::to_string(this->exp * 100 / this->expNext) + "%";
		else
			return"0%";
	}
	inline const int& getHp()const { return this->hp; }
	inline const int& getHpMax()const { return this->hpMax; }
	inline bool isAlive()const { return this->hp > 0; }
	void gainHP(int hp);
	void upgradeHP();
	void upgradeArmor() { this->armor++; }
	int getDamage()const;
	void takeDamage(int damage);

	inline const int& getPlayerNr()const { return this->playerNr; }
	
	inline const int& getLevel()const { return this->level; }
	inline const int& getExp()const { return this->exp; }
	inline const int& getExpNext()const { return this->expNext; }
	bool gainExp(int exp);
	inline void addStatPoint() { this->statPoints++; }
	void addStatPointRandom();
	bool playerShowStatsIsPressed();
	std::string getStatsAsString()const;
	
	inline void gainScore(int score) { this->score += score; }
	inline const int getScore()const { return this->score; }
	inline int getArmor() { return this->armor; }
	inline bool isDamageCooldown()const { return this->damageTimer < this->damageTimerMax; }

	void setGunLevel(int gunLevel);
	inline const int& getGunLevel()const { return this->mainGunLevel; }
	inline void enablePiercingShot() { this->piercingShot = true; }
	inline bool getPiercingShot()const { return this->piercingShot; }
	inline void enableDualMissiles01() { this->dualMissiles01 = true; }
	inline void enableDualMissiles02() { this->dualMissiles02 = true; }
	inline dArr<int>& getAcquiredUpgrades() { return this->upgradesAcquired; }

	inline float getPowerupTimer()const { return this->powerupTimer; }
	inline float getPowerupTimerMax()const { return this->powerupTimerMax; }
	inline bool getPowerupRF()const { return this->powerupRF; }
	inline bool getPowerupXP()const { return this->powerupXP; }
	inline void enablePowerupRF() { this->powerupRF = true; this->powerupTimer = this->powerupTimerMax; }
	inline void enablePowerupXP() { this->powerupXP = true; this->powerupTimer = this->powerupTimerMax; }

	//Functions
	void reset();
	bool updateLeveling();
	void updateStats();
	void changeAccessories();
	void updateAccessories(const float &dt, const float scrollSpeed);
	void updatePowerups();
	void combat(const float &dt);
	void movement(View &view, const float &dtconst, float scrollSpeed);
	void update(View &view, const float &dt, const float scrollSpeed);
	void draw(RenderTarget &target);

	//Regular functions
	float vectorLength(Vector2f v)
	{
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}

	Vector2f normalize(Vector2f v, float length)
	{
		if (length == 0)
			return Vector2f(0.f, 0.f);
		else
			return v / length;
	}

	//Statics
	static unsigned players;

	//Static Textures
	static dArr<Texture> bodyTextures;
	static dArr<Texture> mainGunTextures;
	static dArr<Texture> lWingTextures;
	static dArr<Texture> rWingTextures;
	static dArr<Texture> cPitTextures;
	static dArr<Texture> auraTextures;

	static void initTextures();

	enum weapons {LASER_NORMAL = 0, MISSILE_LIGHT, MISSILE_HEAVY };
	enum controls { UP = 0, DOWN, LEFT, RIGHT, SHOOT, STATS };
};

