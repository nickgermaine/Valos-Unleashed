#include <iostream>
#include <cstdlib>
#include <c++/vector>

using namespace std;

// Items
class Item {
public:
    string name, type, description;
    int prop, quantity;
    Item(string, string, string, int, int);
};

Item::Item(string iname, string idescription, string itype, int iprop, int iquant){
    name = iname;
    description = idescription;
    type = itype;
    prop = iprop;
    quantity = iquant;
}


// Magics
class Spell {
public:
    string name, type;
    int dmg, cost;
    Spell(string, string, int, int);

};

Spell::Spell(string iname, string itype, int idmg, int icost){
    name = iname;
    type = itype;
    dmg = idmg;
    cost = icost;
}


// Characters
class Actor {
    public:
        Actor(int, int, int, int, int, int, int, vector<Spell>&, vector<Item>&);

        int hp, max_hp, mp, max_mp, str, vit, df;
        vector<Spell> magic;
        vector<Item> items;

        void take_damage(int);
        int generate_damage();
        void choose_action();
        void choose_spell(vector<Spell>&);
        void choose_item();

        int generate_spell_damage(Spell);
        void reduce_mp(Spell);
        void heal(int);

};

Actor::Actor(int ihp, int imax_hp, int imp, int imax_mp, int istr, int ivit, int idf, vector<Spell>& imag, vector<Item>& iitem){
    hp = ihp;
    max_hp = imax_hp;
    mp = imp;
    max_mp = imax_mp;
    str = istr;
    vit = ivit;
    df = idf;
    magic = imag;
    items = iitem;
}

// Create some magics:
Spell fire("Fire", "black", 530, 64);
Spell blizzard("Blizzard", "black", 530, 64);
Spell thunder("Thunder", "black", 530, 64);
Spell meteor("Meteor", "black", 762, 120);
Spell quake("Quake", "black", 342, 43);
Spell cure("Cure", "white", 1022, 60);

vector<Spell> Magic = {fire, blizzard, thunder, meteor, cure};
vector<Spell> EnemyMagic = {thunder, quake};


// Create some items:
Item potion("Potion", "Heals 50 HP", "potion", 50, 5);
Item hipotion("Hi-Potion", "Heals 500 HP", "potion", 500, 15);
Item maxpotion("Max Potion", "Heals 1000 HP", "potion", 1000, 5);
Item elixer("Elixer", "Restores full HP and MP", "elixer", 9999, 2);
Item grenade("Grenade", "Deals 1000 damage to single enemy", "attack", 1000, 2);

vector<Item> Items = {potion, hipotion, maxpotion, elixer, grenade};

vector<Item> EnemyItems = {hipotion, elixer};


Actor player(3135, 3135, 412, 412, 325, 96, 93, Magic, Items);
Actor enemy(3477, 3477, 522, 522, 339, 87, 85, EnemyMagic, EnemyItems);


int Actor::generate_damage(){
    int low = player.str - 10;
    int damage = rand() % 10 + low;
    return damage;
}

void Actor::take_damage(int dmg){
    int to_take = dmg - df;
    hp = hp - to_take;

    if(hp < 0){
        hp = 0;
    }
}

void Actor::choose_action(){
    cout << "ACTION:" << endl;
    cout << "    1. Attack" << endl;
    cout << "    2. Magic" << endl;
    cout << "    3. Item" << endl;
    cout << "Choose: ";

}

void Actor::choose_spell(vector<Spell>& spells){
    cout << "MAGIC:" << endl;

    for(int i = 0; i <= spells.size() - 1; i++){
        cout << "    " << to_string(i + 1) << ". " << spells[i].name << " (" << to_string(spells[i].cost) << ")" << endl;
    }

    cout << "Choose spell: " << endl;
}

void Actor::choose_item(){
    cout << "ITEMS:" << endl;


    for(int i = 0; i <= items.size() - 1; i++){
        cout << "    " << to_string(i + 1) << ". " << items[i].name << ": " << items[i].description << " (x"
                 << to_string(items[i].quantity) << ")" << endl;

    }

    cout << "Choose item: " << endl;
}

int Actor::generate_spell_damage(Spell spell){
    int md_low = spell.dmg - 20;
    int damage = rand() % 40 + md_low;
    return damage;
}

void Actor::reduce_mp(Spell spell){
    mp -= spell.cost;

    if(mp < 0){
        mp = 0;
    }
}

void Actor::heal(int dmg){
    hp += dmg;
    if(hp > max_hp){
        hp = max_hp;
    }
}


int main() {
    bool running = true;

    cout << "An enemy attacks!" << endl << endl;

    while(running == true) {
        cout << "=====================================" << endl << endl;

        player.choose_action();
        int action;
        cin >> action;

        cout << endl;

        if(action == 1){
            // CHOSE ATTACK
            int generated_damage = player.generate_damage() - enemy.df;
            enemy.take_damage(generated_damage);
            cout << "You attack for " << to_string(generated_damage) << " points of damage." << endl;
        }else if(action == 0){
            // INVALID BUT QUITS
            running = false;
        }else if(action == 2){
            // CHOSE MAGIC
            player.choose_spell(player.magic);
            int spell;
            cin >> spell;

            if(spell == 0){
                continue;
            }

            spell -= 1;


            if(player.mp >= player.magic[spell].cost) {
                int magic_damage = player.generate_spell_damage(Magic[spell]);
                cout << endl;

                if(Magic[spell].type == "black") {
                    enemy.take_damage(magic_damage);
                    cout << player.magic[spell].name << " deals " << magic_damage << " magic damage." << endl;
                }else if(Magic[spell].type == "white"){
                    player.heal(magic_damage);
                    cout << player.magic[spell].name << " heals for " << magic_damage << " HP." << endl;
                }

                player.reduce_mp(player.magic[spell]);
            }else{
                cout << endl << "Not enough MP." << endl;
                continue;
            }

        }else if(action == 3){
            //Chose Item
            player.choose_item();
            int item;
            cin >> item;

            if(item == 0){
                continue;
            }

            item -= 1;

            if(player.items[item].quantity <= 0){
                cout << "None left..." << endl;
                continue;
            }

            player.items[item].quantity -= 1;
            if(player.items[item].type == "potion"){
                player.heal(player.items[item].prop);
                cout << player.items[item].name << " heals for " << to_string(player.items[item].prop) << " HP." << endl;
            }else if(player.items[item].type == "elixer"){
                player.hp = player.max_hp;
                player.mp = player.max_mp;

                cout << player.items[item].name << " fully restores your HP and MP." << endl;
            }else if(player.items[item].type == "attack"){
                enemy.take_damage(player.items[item].prop);

                cout << player.items[item].name << " deals " << to_string(player.items[item].prop) << " points of damage." << endl;
            }
        }

        cout << "Player HP: " << player.hp << "/" << player.max_hp << " -- MP: " << player.mp << "/" << player.max_mp << endl;
        cout << "Enemy HP: " << enemy.hp << "/" << enemy.max_hp << endl;

        if(enemy.hp == 0){
            cout << endl << "YOU WIN!" << endl;
            running = false;
        }else if(player.hp == 0){
            cout << endl << "The world fades to black... your enemy has defeated you." << endl;
            running = false;
        }
    }


    return 0;
}