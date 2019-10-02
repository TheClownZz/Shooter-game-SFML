#include "Player.h"

unsigned Player::players = 0;

dArr<Texture> Player::bodyTextures;
dArr<Texture> Player::mainGunTextures;
dArr<Texture> Player::lWingTextures;
dArr<Texture> Player::rWingTextures;
dArr<Texture> Player::cPitTextures;
dArr<Texture> Player::auraTextures;

sf::SoundBuffer Player::soundfireB;
sf::Sound Player::soundFire;

void Player::initTextures()
{
	Texture temp;
	//Init textures regular
	temp.loadFromFile("Textures/ship.png");
	Player::bodyTextures.add(temp);
	//Player guns
	temp.loadFromFile("Textures/Guns/gun01.png");
	Player::mainGunTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Guns/gun02.png");
	Player::mainGunTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Guns/gun03.png");
	Player::mainGunTextures.add(Texture(temp));
	//Init accessory textures
	std::ifstream in;
	in.open("Textures/Accessories/leftwings.txt");
	std::string fileName = "";
	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			temp.loadFromFile(fileName);
			Player::lWingTextures.add(Texture(temp));
		}
	}
	in.close();
	in.open("Textures/Accessories/rightwings.txt");
	fileName = "";
	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			temp.loadFromFile(fileName);
			Player::rWingTextures.add(Texture(temp));
		}
	}
	in.close();
	in.open("Textures/Accessories/cockpits.txt");
	fileName = "";
	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			temp.loadFromFile(fileName);
			Player::cPitTextures.add(Texture(temp));
		}
	}
	in.close();
	in.open("Textures/Accessories/auras.txt");
	fileName = "";
	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			temp.loadFromFile(fileName);
			Player::auraTextures.add(Texture(temp));
		}
	}
	in.close();
}

Player::Player(
	int UP,
	int DOWN,
	int LEFT,
	int RIGHT,
	int SHOOT,
	int STATS)

	:level(1), exp(0),
	hp(START_HP), hpMax(START_HP),hpAdded(ADD_HP),
	statPoints(0), armor(0), 
	plating(0), agility(0), power(0),
	damage(1), damageMax(MAX_DAME),
	score(0)
{
	//Dt
	this->dtMultiplier = DT_MUTILPLIER;

	//Add number of players for coop
	this->playerNr = Player::players;
	Player::players++;

	//Keytime
	this->keyTimeMax = 10.f;
	this->keyTime = this->keyTimeMax;

	//Stats
	this->expNext = 20 + static_cast<int>(
		(50 / 3)
		*((pow(level, 3) - 6 
			* pow(level, 2)) + 17 
			* level - 12)
		);

	//Textures & Sprites
	this->sprite.setTexture(Player::bodyTextures[0]);
	this->sprite.setScale(0.08f, 0.08f);
	this->sprite.setColor(Color(10, 10, 10, 255));
	this->sprite.setPosition(Vector2f(400.f, 300.f + Player::playerNr * 150.f));

	//Update positions
	this->playerCenter.x = this->sprite.getPosition().x +
		this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y +
		this->sprite.getGlobalBounds().height / 2;

	//Init main gun
	this->mainGunSprite.setTexture(Player::mainGunTextures[0]);
	this->mainGunSprite.setOrigin(
		this->mainGunSprite.getGlobalBounds().width / 2,
		this->mainGunSprite.getGlobalBounds().height / 2
	);

	this->mainGunSprite.rotate(90);

	this->mainGunSprite.setPosition(
		this->playerCenter.x + 20.f,
		this->playerCenter.y
	);
	this->mainGunSprite.setScale(0.9f, 0.9f);

	//Sound
	if (!this->soundfireB.loadFromFile("Sound/soundFire.wav"))
		return;
	this->soundFire.setBuffer(soundfireB);

	//Accessories

	//Selectors
	this->SkinSelect = 1;

	//Acceccory textures
	this->lWing.setTexture(Player::lWingTextures[this->SkinSelect]);
	this->rWing.setTexture(Player::rWingTextures[this->SkinSelect]);
	this->cPit.setTexture(Player::cPitTextures[this->SkinSelect]);
	this->aura.setTexture(Player::auraTextures[this->SkinSelect]);

	//Init accessories
	float accScale = 0.7f;
	this->lWing.setOrigin(
		this->lWing.getGlobalBounds().width / 2,
		this->lWing.getGlobalBounds().height / 2
		);

	this->lWing.setPosition(this->playerCenter);
	this->lWing.setRotation(90.f);
	this->lWing.setScale(accScale, accScale);

	this->rWing.setOrigin(
		this->rWing.getGlobalBounds().width / 2,
		this->rWing.getGlobalBounds().height / 2
	);

	this->rWing.setPosition(this->playerCenter);
	this->rWing.setRotation(90.f);
	this->rWing.setScale(accScale, accScale);

	this->cPit.setOrigin(
		this->cPit.getGlobalBounds().width / 2,
		this->cPit.getGlobalBounds().height / 2
		);

	this->cPit.setPosition(this->playerCenter);
	this->cPit.setRotation(90.f);
	this->cPit.setScale(accScale, accScale);

	this->aura.setOrigin(
		this->aura.getGlobalBounds().width / 2,
		this->aura.getGlobalBounds().height / 2
		);

	this->aura.setPosition(this->playerCenter);
	this->aura.setRotation(90.f);
	this->aura.setScale(accScale, accScale);

	//Timers
	this->shootTimerMax = SHOOT_TIMER_MAX;
	this->shootTimer = this->shootTimerMax;
	this->damageTimerMax = DAME_TIMER_MAX;
	this->damageTimer = this->damageTimerMax;
	this->powerupTimerMax = POWERUP_TIMER_MAX;
	this->powerupTimer = 0.f;

	//Controls
	//FOLLOW CONTROLS ENUM TEMPLATE!
	this->controls.add(int(UP));
	this->controls.add(int(DOWN));
	this->controls.add(int(LEFT));
	this->controls.add(int(RIGHT));
	this->controls.add(int(SHOOT));
	this->controls.add(int(STATS));

	//Velocity & Acceleration
	this->maxVelocity = 25.f;
	this->acceleration = 0.8f;
	this->stabilizerForce = 0.3f;

	//Guns
	this->currentWeapon = Player::LASER_NORMAL;

	//Upgrades
	this->mainGunLevel = 0;
	this->piercingShot = false;
	this->dualMissiles01 = false;
	this->dualMissiles02 = false;

	this->setGunLevel(0);

	//Powerups
	this->powerupRF = false;
	this->powerupXP = false;

	//Sound
	if (!soundfireB.loadFromFile("Sound/soundFire.wav")){}
	else
		this->soundFire.setBuffer(soundfireB);

}

Player::~Player()
{

}

void Player::setPos(float x, float y)
{
	this->sprite.setPosition(Vector2f(x, y)); this->mainGunSprite.setPosition(Vector2f(x, y));
	//Update positions
	this->playerCenter.x = this->sprite.getPosition().x +
		this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y +
		this->sprite.getGlobalBounds().height / 2;
}

int Player::getDamage()const
{ 
	int damage = 0;

	switch (this->currentWeapon)
	{
	case Player::LASER_NORMAL:

		damage = rand() % this->damageMax + this->damage;

		break;
	case Player::MISSILE_LIGHT:

		damage = rand() % this->damageMax + this->damage;
		damage *= 2;

		break;
	case Player::MISSILE_HEAVY:

		damage = rand() % this->damageMax + this->damage;
		damage *= 4;

		break;
	default:

		damage = rand() % this->damageMax + this->damage;

		break;
	}

	return damage;
}

void Player::takeDamage(int damage)
{ 
	this->hp -= damage; 

	this->damageTimer = 0;

	this->currentVelocity.x += -this->normDir.x*10.f;
	this->currentVelocity.y += -this->normDir.y*10.f;
}

bool Player::updateLeveling()
{
	if (this->exp >= this->expNext)
	{
		this->level++;
		this->statPoints++;
		this->exp -= this->expNext;
		this->expNext = static_cast<int>((50 / 3)*((pow(level, 3) - 6 * pow(level, 2)) + 17 * level - 12));

		this->agility++;
		this->plating++;
		this->power++;
		upgradeHP();

		changeAccessories();
		return true;
	}

	return false;
}

void Player::updateStats()
{
	this->hpMax =START_HP+ this->hpAdded + this->plating*5;
	this->damageMax = MAX_DAME + this->power * 2;
	this->damage = 1 + this->power;
}

void Player::changeAccessories()
{
	if (this->level % 4 != 0)
		return;
	SkinSelect++;
	armor++;
	//left
	if(SkinSelect<= Player::lWingTextures.size()-1)
		this->lWing.setTexture(Player::lWingTextures[SkinSelect]);
	else
		this->lWing.setTexture(Player::lWingTextures[Player::lWingTextures.size() - 1]);

	//right
	if (SkinSelect <= Player::rWingTextures.size() - 1)
		this->rWing.setTexture(Player::rWingTextures[SkinSelect]);
	else
		this->rWing.setTexture(Player::rWingTextures[Player::rWingTextures.size() - 1]);

	//cpit
	if (SkinSelect <= Player::cPitTextures.size() - 1)
		this->cPit.setTexture(Player::cPitTextures[SkinSelect]);
	else
		this->cPit.setTexture(Player::cPitTextures[Player::cPitTextures.size() - 1]);

	//aura
	if (SkinSelect <= Player::auraTextures.size() - 1)
		this->aura.setTexture(Player::auraTextures[SkinSelect]);
	else
		this->aura.setTexture(Player::auraTextures[Player::auraTextures.size() - 1]);
}

void Player::updateAccessories(const float &dt, const float scrollSpeed)
{
	//Set the position of gun to follow player
	this->mainGunSprite.setPosition(
		this->mainGunSprite.getPosition().x,
		this->playerCenter.y
	);


	//Animate the main gun and correct it after firing
	if (this->mainGunSprite.getPosition().x < this->playerCenter.x + 20.f)
	{
		this->mainGunSprite.move(
			scrollSpeed * dt * (this->dtMultiplier + SHOCK_TIME)
			+ this->currentVelocity.x * dt * this->dtMultiplier, 0.f);
	}
	if (this->mainGunSprite.getPosition().x > this->playerCenter.x + 20.f)
	{
		this->mainGunSprite.setPosition(
			this->playerCenter.x + 20.f,
			this->playerCenter.y
		);
	}

	//Left wing
	this->lWing.setPosition(
		playerCenter.x + -abs(this->currentVelocity.x), 
		playerCenter.y + -abs(this->currentVelocity.x/2 + this->currentVelocity.y/2)
	);

	//Right wing
	this->rWing.setPosition(
		playerCenter.x + -abs(this->currentVelocity.x), 
		playerCenter.y + abs(this->currentVelocity.x / 2 + this->currentVelocity.y / 2)
	);

	//Cockpit
	this->cPit.setPosition(
		playerCenter.x + this->currentVelocity.x, 
		playerCenter.y
	);

	//Aura
	this->aura.setPosition(playerCenter);
	this->aura.rotate(3.f * dt * this->dtMultiplier);
}

void Player::updatePowerups()
{
	if (this->powerupTimer <= 0.f)
	{
		this->powerupRF = false;
		this->powerupXP = false;
	}
}

void Player::movement(View &view, const float &dt, const float scrollSpeed)
{
	//Update normalized direction
	this->normDir = normalize(this->currentVelocity, vectorLength(this->currentVelocity));

	//UP
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::UP])))
	{
		this->direction.x = 0.f;
		this->direction.y = -1.f;
	
		if (this->currentVelocity.y > -this->maxVelocity && this->direction.y < 0)
			this->currentVelocity.y += this->direction.y * this->acceleration
			* dt * this->dtMultiplier;
	}
	
	//DOWN
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::DOWN])))
	{ 
		this->direction.x = 0.f;
		this->direction.y = 1.f;

		if (this->currentVelocity.y < this->maxVelocity && this->direction.y > 0)
			this->currentVelocity.y += this->direction.y * this->acceleration
			* dt * this->dtMultiplier;
	}
	
	//LEFT
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::LEFT])))
	{ 
		this->direction.x = -1.f;
		this->direction.y = 0.f;

		if (this->currentVelocity.x > -this->maxVelocity && this->direction.x < 0)
			this->currentVelocity.x += this->direction.x * this->acceleration
			* dt * this->dtMultiplier;

	}
	
	//RIGHT
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::RIGHT])))
	{ 
		this->direction.x = 1.f;
		this->direction.y = 0.f;

		if (this->currentVelocity.x < this->maxVelocity && this->direction.x > 0)
			this->currentVelocity.x += this->direction.x * this->acceleration
			* dt * this->dtMultiplier;

	}

	//Drag force
	if (this->currentVelocity.x > 0)
	{
		this->currentVelocity.x -= this->stabilizerForce
			* dt * this->dtMultiplier;

		if (this->currentVelocity.x < 0)
			this->currentVelocity.x = 0;
	}
	else if (this->currentVelocity.x < 0)
	{
		this->currentVelocity.x += this->stabilizerForce
			* dt * this->dtMultiplier;

		if (this->currentVelocity.x > 0)
			this->currentVelocity.x = 0;
	}
	if (this->currentVelocity.y > 0)
	{
		this->currentVelocity.y -= this->stabilizerForce
			* dt * this->dtMultiplier;

		if (this->currentVelocity.y < 0)
			this->currentVelocity.y = 0;
	}
	else if (this->currentVelocity.y < 0)
	{
		this->currentVelocity.y += this->stabilizerForce
			* dt * this->dtMultiplier;

		if (this->currentVelocity.y > 0)
			this->currentVelocity.y = 0;
	}

	//Final move
	this->sprite.move(
		scrollSpeed * dt * this->dtMultiplier +
		this->currentVelocity.x * dt * this->dtMultiplier, 
		this->currentVelocity.y * dt * this->dtMultiplier);

	//Update positions
	this->playerCenter.x = this->sprite.getPosition().x +
		this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y +
		this->sprite.getGlobalBounds().height / 2;

	//Window collision
	//Left
	if (this->getPosition().x <= view.getCenter().x - view.getSize().x/2)
	{
		this->sprite.setPosition(view.getCenter().x - view.getSize().x / 2 + 10.f, this->sprite.getPosition().y);
		this->currentVelocity.x = 0.f;
	}
	//Right
	else if (this->getPosition().x + this->getBounds().width >= view.getCenter().x + view.getSize().x / 2)
	{
		this->sprite.setPosition(view.getCenter().x + view.getSize().x / 2 - this->getBounds().width, this->sprite.getPosition().y);
		this->currentVelocity.x = 0.f;
	}	
	//Top
	if (this->getPosition().y <= view.getCenter().y - view.getSize().y / 2)
	{
		this->sprite.setPosition(this->sprite.getPosition().x, view.getCenter().y - view.getSize().y / 2);
		this->currentVelocity.y = 0.f;
	}
	//Bottom
	else if (this->getPosition().y + this->getBounds().height >= view.getCenter().y + view.getSize().y / 2)
	{
		this->sprite.setPosition(this->sprite.getPosition().x, view.getCenter().y + view.getSize().y / 2 - this->getBounds().height);
		this->currentVelocity.y = 0.f;
	}	
}

void Player::combat(const float &dt)
{
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::SHOOT]))
		&& this->shootTimer >= this->shootTimerMax)
	{
		this->soundFire.play();

		if (this->currentWeapon == Player::LASER_NORMAL)
		{
			//Create bullet
			if (this->mainGunLevel == 0)
			{
				this->bullets.add(
					Bullet(
						Bullet::LASER_RED,
						Vector2f(this->playerCenter.x + 100.f, this->playerCenter.y),
						Vector2f(0.15f, 0.15f),
						Vector2f(1.f, 0.f),
						20.f, 
						60.f, 
						5.f, 
						this->getDamage()
					));
			}
			else if (this->mainGunLevel == 1)
			{
				this->bullets.add(
					Bullet(
						Bullet::LASER_RED,
						Vector2f(this->playerCenter.x + 100.f, this->playerCenter.y - 15.f),
						Vector2f(0.2f, 0.2f),
						Vector2f(1.f, 0.f),
						20.f, 
						60.f, 
						5.f,
						this->getDamage()
					)
				);

				this->bullets.add(
					Bullet(
						Bullet::LASER_RED,
						Vector2f(this->playerCenter.x + 100.f, this->playerCenter.y + 15.f),
						Vector2f(0.2f, 0.2f),
						Vector2f(1.f, 0.f),
						20.f, 
						60.f, 
						5.f,
						this->getDamage()
					)
				);
			}
			else if (this->mainGunLevel == 2)
			{
				this->bullets.add(
					Bullet(
						Bullet::LASER_RED,
						Vector2f(this->playerCenter.x + 100.f, this->playerCenter.y - 41.f),
						Vector2f(0.2f, 0.2f),
						Vector2f(1.f, 0.f),
						20.f, 
						60.f,
						5.f,
						this->getDamage()
					)
				);

				this->bullets.add(
					Bullet(
						Bullet::LASER_RED,
						Vector2f(this->playerCenter.x + 100.f, this->playerCenter.y),
						Vector2f(0.2f, 0.2f),
						Vector2f(1.f, 0.f),
						20.f, 
						60.f, 
						5.f,
						this->getDamage()
					)
				);

				this->bullets.add(
					Bullet(
						Bullet::LASER_RED,
						Vector2f(this->playerCenter.x + 100.f, this->playerCenter.y + 43.f),
						Vector2f(0.2f, 0.2f),
						Vector2f(1.f, 0.f),
						20.f, 
						60.f, 
						5.f,
						this->getDamage()
					)
				);
			}

			//Animate gun
			//if (this->mainGunSprite.getPosition().x >= this->getPosition().x + 40)
				this->mainGunSprite.move(-35.0f*(this->agility < 100 ? 1 : 100.0f / this->agility), 0.f);
		}
		else if (this->currentWeapon == Player::MISSILE_LIGHT)
		{
			//Create bullet
			this->bullets.add(
				Bullet(
					Bullet::MISSILE_LIGHT_RIGHT,
					Vector2f(this->playerCenter.x, this->playerCenter.y - 25.f),
					Vector2f(0.05f, 0.05f),
					Vector2f(1.f, 0.f), 
					0.f,
					50.f,
					1.f,
					this->getDamage()
				)
			);

			if (this->dualMissiles01)
			{
				this->bullets.add(
					Bullet(
						Bullet::MISSILE_LIGHT_RIGHT,
						Vector2f(this->playerCenter.x, this->playerCenter.y + 25.f),
						Vector2f(0.05f, 0.05f),
						Vector2f(1.f, 0.f),
						0.f, 
						50.f,
						1.f,
						this->getDamage()
					)
				);
			}
		}
		else if (this->currentWeapon == Player::MISSILE_HEAVY)
		{
			if (this->dualMissiles02)
			{
				
			}
		}
	
		this->shootTimer = 0; //RESET TIMER!
	}


	//DAMAGED
	if (this->isDamageCooldown())
	{
		if ((int)this->damageTimer % 2 == 0)
		{
			this->lWing.setColor(Color::Red);
			this->rWing.setColor(Color::Red);
			this->cPit.setColor(Color::Red);
		}
		else
		{
			this->lWing.setColor(Color::White);
			this->rWing.setColor(Color::White);
			this->cPit.setColor(Color::White);
		}
	}
	else
	{
		this->lWing.setColor(Color::White);
		this->rWing.setColor(Color::White);
		this->cPit.setColor(Color::White);
	}
}

Bullet& Player::getBullet(unsigned index)
{
	if (index < 0 || index > this->bullets.size())
		throw"OUT OF BOUNDS! PLAYER::GETBULLET!";

	return this->bullets[index];
}

void Player::removeBullet(unsigned index)
{
	if (index < 0 || index > this->bullets.size())
		throw"OUT OF BOUNDS! PLAYER::REMOVEBULLET!";

	this->bullets.remove(index);
}

void Player::setGunLevel(int gunLevel) 
{ 
	this->mainGunLevel = gunLevel; 

	if (this->mainGunLevel < Player::mainGunTextures.size())
		this->mainGunSprite.setTexture(Player::mainGunTextures[this->mainGunLevel]);
	else
		std::cout << "NO TEXTURE FOR THAT MAIN GUN!" << "\n";
}

void Player::addStatPointRandom()
{
	int r = rand() % 4;
	switch (r)
	{
	case 0:
		this->power++;
		break;
	case 1:
		this->agility++;
		break;
	case 2:
		this->armor++;
		break;
	case 3:
		this->plating++;
		break;
	default:
		break;
	}

	this->updateStats();
}

bool Player::gainExp(int exp)
{
	this->exp += exp;
	return this->updateLeveling();
}

void Player::gainHP(int hp)
{
	this->hp += hp;
	if (this->hp > this->hpMax)
		this->hp = this->hpMax;
}

void Player:: upgradeHP() 
{ 
	this->hpAdded += ADD_HP; 
	this->updateStats(); 
	this->hp = this->hpMax; 
}

bool Player::playerShowStatsIsPressed()
{
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::STATS])))
		return true;

	return false;
}

std::string Player::getStatsAsString()const
{
	return
		"Level: " + std::to_string(this->level) +
		"\nExp: " + std::to_string(this->exp) + " / " + std::to_string(this->expNext) +
		"\nStatpoints: " + std::to_string(this->statPoints) +
		"\nHP: " + std::to_string(this->hp) + " / " + std::to_string(this->hpMax) + " ( +" + std::to_string(this->hpAdded) + ") "
		"\nDamage: " + std::to_string(this->damage) + " - " + std::to_string(this->damageMax) +
		"\n\nPower: " + std::to_string(this->power) +
		"\nPlating: " + std::to_string(this->plating) +
		"\nagility: " + std::to_string(this->agility) +
		"\narmor: " + std::to_string(this->armor);
}

void Player::reset()
{
	//Reset sprites
	this->sprite.setPosition(Vector2f(400.f, 300.f + Player::playerNr * 150.f));

	//Reset stats
	this->hpMax = START_HP;
	this->hpAdded = ADD_HP;
	this->hp = this->hpMax;
	this->agility = 0;
	this->armor = 0;
	this->power = 0;
	this->plating = 0;
	this->level = 1;
	this->exp = 0;
	this->expNext = 0;
	this->statPoints = 0;
	this->score = 0;
	//this->updateStats();
	this->SkinSelect = 1;

	//Reset Accessories 
	this->lWing.setTexture(Player::lWingTextures[this->SkinSelect]);
	this->rWing.setTexture(Player::rWingTextures[this->SkinSelect]);
	this->cPit.setTexture(Player::cPitTextures[this->SkinSelect]);
	this->aura.setTexture(Player::auraTextures[this->SkinSelect]);

	//Reset physics and movement
	this->currentVelocity.x = 0;
	this->currentVelocity.y = 0;

	//Reset arrays
	this->bullets.clear();
	this->upgradesAcquired.clear();

	//Reset weapon
	this->setGunLevel(Player::LASER_NORMAL);
	this->currentWeapon = LASER_NORMAL;

	//Reset upgrades
	this->dualMissiles01 = false;
	this->dualMissiles02 = false;
	this->piercingShot = false;

	//Reseet powerups
	this->powerupRF = false;
	this->powerupXP = false;

	//Reset timers
	this->shootTimer = this->shootTimerMax;
	this->damageTimer = this->damageTimerMax;
	this->powerupTimer = 0;
}

void Player::update(View &view, const float &dt, const float scrollSpeed)
{
	//Update timers
	if (this->powerupRF)
	{
		this->shootTimerMax = SHOOT_TIMER_MAX / 2;
		if (this->shootTimer < this->shootTimerMax)
			this->shootTimer += 1.f * dt * (this->dtMultiplier + this->agility);
	}
	else
	{
		this->shootTimerMax = SHOOT_TIMER_MAX;
		if (this->shootTimer < this->shootTimerMax)
			this->shootTimer += 1.f * dt * (this->dtMultiplier + this->agility);
	}

	if (this->damageTimer < this->damageTimerMax)
		this->damageTimer += 1.f * dt * this->dtMultiplier;

	if (this->powerupTimer > 0.f)
		this->powerupTimer -= 1.f * dt * this->dtMultiplier;

	this->movement(view, dt, scrollSpeed);
	this->updateAccessories(dt, scrollSpeed);
	this->updatePowerups();
	this->combat(dt);
}

void Player::draw(RenderTarget &target)
{
	target.draw(this->aura);

	for (size_t i = 0; i < this->bullets.size(); i++)
	{
		this->bullets[i].Draw(target);
	}

	target.draw(this->mainGunSprite);

	target.draw(this->sprite);

	target.draw(this->cPit);
	target.draw(this->lWing);
	target.draw(this->rWing);

}