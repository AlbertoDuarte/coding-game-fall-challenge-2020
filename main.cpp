#include <bits/stdc++.h>

using namespace std;

const int NUM_TIERS = 4;
const int MAX_INGREDIENTS = 10;

struct Spell {
    vector<int> ing;
    int id;
    bool castable;

    Spell() {
        id = -1;
    }

    Spell(vector<int> ingredients, bool castable, int id) {
        this->ing = ingredients;
        this->castable = castable;
        this->id = id;
    }

};

struct Order {
    vector<int> ing;
    int price, id;

    Order() {
        id = -1;
    }

    Order(vector<int> ingredients, int price, int id) {
        this->ing = ingredients;
        this->price = price;
        this->id = id;
    }

    bool operator<(Order other) const {
        return this->price < other.price;
    }

};

struct Player {
    vector<int> ing;
    int score;

    Player(){}

    Player(vector<int> ingredients, int score) {
        this->ing = ingredients;
        this->score = score;
    }
};

bool permCompare(const pair<int, Spell> a, const pair<int, Spell> b) {
    return a.first < b.first;
}

bool bestOrder(vector<Order> orders, Order& order_best, Player player, bool onlyPossible = true) {
    // get best possible potion
    int best_price = -1;

    for(Order order : orders) {
        bool canbuy = true;
        for(int i = 0; i < NUM_TIERS; i++) {
            if(-order.ing[i] > player.ing[i]) {
                canbuy = false;
                break;
            }
        }

        if((canbuy or !onlyPossible) and order.price > best_price) {
            // cerr << "order " << order.id << " is possible " << endl;
            // for(int i = 0; i <= 3; i++) cerr << "t " << i << " = " << order.ing[i] << endl;
            best_price = order.price;
            order_best = order;
        }
    }

    return (best_price != -1);
}

int nextSpell(vector<Spell> spells, Order order, Player player, Spell& next_spell) {
    if(!spells.size()) return -1;
    assert(order.id != -1);
    vector<pair<int, Spell>> perm;

    for(int i = 0; i < (int)spells.size(); i++) {
        if(spells[i].castable)
            perm.push_back({i, spells[i]});
    }

    vector<int> needed(NUM_TIERS, 0);
    vector<int> ing = player.ing;
    for(int i = NUM_TIERS-1; i >= 0; i--) {
        if(-order.ing[i]+needed[i] > ing[i]) {
            needed[i] += -order.ing[i] - ing[i];
            if(i > 0) {
                int multiplier = (i == 1 ? 2 : 1);
                needed[i-1] += multiplier*needed[i];
            }
        }
    }

    cerr << "needed" << endl;
    for(int i = 0; i < NUM_TIERS; i++) {
        cerr << needed[i] << " ";
    }
    cerr << endl;

    cerr << "order" << endl;
    for(int i = 0; i < NUM_TIERS; i++) {
        cerr << order.ing[i] << " ";
    }
    cerr << endl;

    reverse(perm.begin(), perm.end());
    for(auto e : perm) {
        Spell spell = e.second;
        bool canCast = true;
        bool want = false;
        int total = 0;
        for(int i = 0; i < NUM_TIERS; i++) {
            total += player.ing[i];
            total += spell.ing[i];
            if(-spell.ing[i] > player.ing[i]) {
                canCast = false;
            }
            if(spell.ing[i] > 0 and needed[i] > 0) {
                want = true;
            }
        }
        if(total > MAX_INGREDIENTS) canCast = false;

        if(want and canCast) {
            next_spell = spell;
            return 1;
        }
    }
    

    return -1;
}

int main() {

    int last_player = 0, last_opponent = 0;
    int potions_player = 0, potions_opponent = 0;
    // game loop
    while (1) {
        int actionCount; // the number of spells and recipes in play
        cin >> actionCount; cin.ignore();

        vector<Order> orders;
        vector<Spell> spells;
        Player player, opponent;

        for (int i = 0; i < actionCount; i++) {
            int actionId; // the unique ID of this spell or recipe
            string actionType; // in the first league: BREW; later: CAST, OPPONENT_CAST, LEARN, BREW
            int delta0; // tier-0 ingredient change
            int delta1; // tier-1 ingredient change
            int delta2; // tier-2 ingredient change
int delta3; // tier-3 ingredient change
            int price; // the price in rupees if this is a potion
            int tomeIndex; // in the first two leagues: always 0; later: the index in the tome if this is a tome spell, equal to the read-ahead tax; For brews, this is the value of the current urgency bonus
            int taxCount; // in the first two leagues: always 0; later: the amount of taxed tier-0 ingredients you gain from learning this spell; For brews, this is how many times you can still gain an urgency bonus
            bool castable; // in the first league: always 0; later: 1 if this is a castable player spell
            bool repeatable; // for the first two leagues: always 0; later: 1 if this is a repeatable player spell
            cin >> actionId >> actionType >> delta0 >> delta1 >> delta2 >> delta3 >> price >> tomeIndex >> taxCount >> castable >> repeatable; cin.ignore();
       
            if(actionType == "CAST")     
                spells.push_back(Spell({delta0, delta1, delta2, delta3}, castable, actionId));
            else if(actionType == "BREW") {
                orders.push_back(Order({delta0, delta1, delta2, delta3}, price, actionId));
                // cerr << "id = " << actionId << endl;
            }
        }

        for (int i = 0; i < 2; i++) {
            int inv0; // tier-0 ingredients in inventory
            int inv1;
            int inv2;
            int inv3;
            int score; // amount of rupees
            cin >> inv0 >> inv1 >> inv2 >> inv3 >> score; cin.ignore();

            // ingredients and score of both players
            if(i == 0)
                player = Player({inv0, inv1, inv2, inv3}, score);
            else
                opponent = Player({inv0, inv1, inv2, inv3}, score);

        }

        // in the first league: BREW <id> | WAIT; later: BREW <id> | CAST <id> [<times>] | LEARN <id> | REST | WAIT
        Order best, best_possible;
        bestOrder(orders, best, player, false);
        bestOrder(orders, best_possible, player, true);

        if(best.id == best_possible.id and best_possible.id != -1) {
            cout << "BREW " << best_possible.id << endl;
            cerr << "Brewing id = " << best_possible.id << endl;
            cerr << best_possible.ing[0] << " " << best_possible.ing[1] << " " << best_possible.ing[2] << " " << best_possible.ing[3] << endl;
        }
        else {
            Spell spell;
            nextSpell(spells, best, player, spell);
            if(spell.id != -1) {
                cout << "CAST " << spell.id << endl;
                cerr << "Casting id = " << spell.id << endl;
                cerr << spell.ing[0] << " " << spell.ing[1] << " " << spell.ing[2] << " " << spell.ing[3] << endl;
            }
            else {
                cout << "REST" << endl;
                cerr << "Waiting" << endl;
            }
        }
    }
}
