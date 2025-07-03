#ifndef BASICSTRUCTS_H
#define BASICSTRUCTS_H 

#include <string>
#include <array>
using namespace std;

#define GREEN "\033[32m"
#define RED "\033[31m"
#define BOLD "\033[1m"
#define RESET "\033[0m"
#define LIGHT_BLUE "\033[94m"
#define DARK_BLUE "\033[34m"
#define PURPLE "\033[35m"
#define LIGHT_GREEN "\033[92m"
#define DARK_GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define WHITE "\033[1;37m"
#define GREY "\033[0;37m"


//Element(0-9): None C H P E A G D Phys Pierc
struct Card          {
    string Name;
    Card(string name) : Name(name){};
};
struct Status        {
    string Name;
    bool Usage_Based;
    int Usages;
    int Rounds;
    int Limit;
    bool Once_Per_Round;
    bool Follow_Up;
    bool TriggerOnCreate;
    bool isPassive;
    int Counter;
    bool Shot = false;
    bool onOpponentSide;

    Status(){ Name = "No Status";}
    Status(string name) : Name(name) {}
    Status(string name, bool usage_based, int amount, int limit)//quicken, bloom core 
        : Name(name), Usage_Based(usage_based), Limit(limit) {
        if (Usage_Based) { Usages = amount; }
        else { Rounds = amount; }
        Once_Per_Round = false;
        Follow_Up = false;
        TriggerOnCreate = false;
        isPassive = false;
        Shot = false;
        onOpponentSide = false;
    };
    Status(string name, bool usage_based, int amount, int limit, bool opr, bool follow_up, bool triggerOnCreate) 
        : Name(name), Usage_Based(usage_based), Limit(limit), Once_Per_Round(opr), Follow_Up(follow_up), TriggerOnCreate(triggerOnCreate) {
        if (Usage_Based) { Usages = amount; }
        else { Rounds = amount; }
        isPassive = false;
        Shot = false;
        onOpponentSide = false;
    }
    Status(string name, bool usage_based, int amount, int limit, bool opr, bool follow_up, bool triggerOnCreate, bool opponentSide)//debuff
        : Name(name), Usage_Based(usage_based), Limit(limit), Once_Per_Round(opr), Follow_Up(follow_up), TriggerOnCreate(triggerOnCreate), onOpponentSide(opponentSide){
        if (Usage_Based) { Usages = amount; }
        else { Rounds = amount; }
        isPassive = false;
        Shot = false;
    }
    Status(string name, int counter, int limit, bool opr, bool follow_up, bool triggerOnCreate, bool ispassive)//passive
        : Name(name), Counter(counter), Limit(limit), Once_Per_Round(opr), Follow_Up(follow_up), TriggerOnCreate(triggerOnCreate), isPassive(ispassive){}
};
struct StatusManager {
    vector<Status> Statuses;
    void printAllStatus(string x) {
        cout << x;
        for (auto &status : Statuses) {
            if(status.isPassive) {
                cout << status.Name << "(" << status.Counter << ")\t";
            }
            else if (status.Usage_Based)  { cout << status.Name << "(" << status.Usages << "x) \t"; }
            else if (!status.Usage_Based) { cout << status.Name << "(" << status.Rounds << "r) \t"; }
        }
        cout << "\t";
    }
    void Round_Update() { // -1 Round, remove if 0
        for (auto it = Statuses.begin(); it != Statuses.end();) {
            if(!it->isPassive) {
                it->Once_Per_Round = true;
                if (!it->Usage_Based) {
                    it->Rounds -= 1;
                    if (it->Rounds <= 0) { it = Statuses.erase(it); } 
                    else { ++it; }
                } 
                else { ++it; }
            }
            else {
                if      (it->Name == "Illusory Torrent"     ) {
                    it->Once_Per_Round = true;
                    it->Counter = 1;
                }
                else if (it->Name == "Pactsworn Pathclearer") {
                    it->Counter += 1;
                    if(it->Counter >= 8) {
                        it->Counter -= 6;
                    }
                }
                else if (it->Name == "DilucE+2"             ) {
                    it->Counter = 0;
                }
                else if (it->Name == "Radical Vitality"     ) {
                    if(it->Counter == it->Limit) {
                        it->Counter = 0;
                    }
                }
                ++it;
            }
        }     
    }

    void Usage_Update(string statusName, int usagesUsed) { 
        for (auto it = Statuses.begin(); it != Statuses.end(); ) {
            if (it->Name == statusName) {
                it->Usages -= usagesUsed;
                if (it->Usages <= 0) {
                    it = Statuses.erase(it);  // Erase safely
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }
    }

    StatusManager() {
        Statuses = {};
    }
};
struct Summon        {
    string Name;
    int Duration;
    int MaxDuration;
    array<int, 3> Dmg;
    array<int, 3> Element;
    bool Offensive;

    array<int, 3> Healing;
    int Cleanse;
    bool Defensive;

    bool Shot = false;

    int Effect_Usages;
    bool Effect_Active;
    bool Once_per_round;

    Summon() : Name("No Summon"), Duration(0), MaxDuration(0), Dmg({0, 0, 0}), Element({0, 0, 0}) {}

    Summon(string name, int duration, int maxDuration, 
        array<int, 3> dmg, array<int, 3> element, bool offensive, array<int, 3> healing, int cleanse, bool defensive)
        : Name(name), Duration(duration), MaxDuration(maxDuration),  
        Dmg(dmg), Element(element), Offensive(offensive), Healing(healing), Cleanse(cleanse), Defensive(defensive) {}

    Summon(string name, int duration, int maxDuration, 
        array<int, 3> dmg, array<int, 3> element, bool offensive, array<int, 3> healing, int cleanse, bool defensive, 
        bool effect_active, int effect_usages)
        : Name(name), Duration(duration), MaxDuration(maxDuration),  
        Dmg(dmg), Element(element), Offensive(offensive), Healing(healing), Cleanse(cleanse), Defensive(defensive), 
        Effect_Active(effect_active), Effect_Usages(effect_usages) {
            Once_per_round = false;
        }
    Summon(string name, int duration, int maxDuration, 
        array<int, 3> dmg, array<int, 3> element, bool offensive, array<int, 3> healing, int cleanse, bool defensive, 
        bool effect_active, int effect_usages, bool opr)
        : Name(name), Duration(duration), MaxDuration(maxDuration),  
        Dmg(dmg), Element(element), Offensive(offensive), Healing(healing), Cleanse(cleanse), Defensive(defensive), 
        Effect_Active(effect_active), Effect_Usages(effect_usages), Once_per_round(opr) {}
    

};
struct SummonZone    {
    int MaxSize = 4;
    vector<Summon> Summons;  

    void Usage_Update(StatusManager &PSM, StatusManager &CSM, string summonName, int DurationUsed) { 
        for (auto it = Summons.begin(); it != Summons.end(); ) {
            if (it->Name == summonName) { 
                it->Duration -= DurationUsed;
                
                if (it->Duration <= 0) {
                    it = Summons.erase(it);  // Safely erase summon
                    printSummons();
                } else {
                    ++it;  // Move forward safely
                }
            } else {
                ++it;  // Move forward safely
            }
        }
    }
    void OPR_Update() {
        for(auto &s : Summons) {
            s.Once_per_round = true;
        }
    }
    void printSummons(){
        for(auto s : Summons) {
            int SummonType   = (s.Offensive ? s.Element[0] : s.Cleanse   ); 
            int SummonNumber = (s.Offensive ?     s.Dmg[0] : s.Healing[0]);
            switch (SummonType) {
                case 1: cout << LIGHT_BLUE; break;   
                case 2: cout << DARK_BLUE; break;    
                case 3: cout << RED; break;         
                case 4: cout << PURPLE; break;       
                case 5: cout << DARK_GREEN; break;
                case 6: cout << YELLOW; break;
                case 7: cout << LIGHT_GREEN; break;  
                case 8: cout << WHITE; break;
                case 9: cout << GREY; break;
            }
            cout << s.Name << "(" << SummonNumber << "," << s.Duration << ")\t" << RESET;
        }
    }
    SummonZone() { Summons = {}; }
};
struct Ability       {
    array<int, 3> Dmg; 
    array<int, 3> Element;
    bool Offensive = false;

    array<int, 3> Healing;
    int Cleanse;
    bool Defensive = false;

    int Dice_Cost;    
    int Energy_Cost; 
    int Energy_Gain;

    int swapEffect; 
    Summon summon; 
    Status C_Status;
    Status P_Status;

    bool IsPrepare;
    Status PrepareSkill;

    bool self = false;

    Ability() {
        Dmg         = {0,0,0};
        Element     = {0,0,0};
        Offensive   = false;
        Healing     = {0,0,0};
        Cleanse     = 0;
        Defensive   = false;
        Dice_Cost   = 100;
        Energy_Cost = 0;
        Energy_Gain = 0;
        summon      = Summon();
        C_Status    = Status();
        P_Status    = Status();
        swapEffect  = 0;
        IsPrepare   = false;
        self        = false;
    }
    Ability(array<int, 3> dmg,array<int, 3> element,bool offensive,array<int, 3> healing,int cleanse,bool defensive) 
            : Dmg(dmg), Element(element), Offensive(offensive), Healing(healing), Cleanse(cleanse), Defensive(defensive){
        Dice_Cost   = 0;
        Energy_Cost = 0;
        Energy_Gain = 0;
        swapEffect  = 0;
        summon      = Summon();
        C_Status    = Status();
        P_Status    = Status();
        
        IsPrepare   = false;
        PrepareSkill = Status();
        self        = false;    
            }; 
    Ability(array<int, 3> dmg,array<int, 3> element,bool offensive,array<int, 3> healing,int cleanse,bool defensive,
            int dice_cost, int energy_cost, int energy_gain, Summon _summon, int _swapEffect, Status c_Status, Status p_Status)
        : Dmg(dmg), Element(element), Offensive(offensive), Healing(healing), Cleanse(cleanse), Defensive(defensive), 
          Dice_Cost(dice_cost), Energy_Cost(energy_cost), Energy_Gain(energy_gain), 
          summon(_summon), swapEffect(_swapEffect), C_Status(c_Status), P_Status(p_Status){
            IsPrepare = false;
            PrepareSkill = Status();
          }
    Ability(array<int, 3> dmg,array<int, 3> element,bool offensive,array<int, 3> healing,int cleanse,bool defensive,
            int dice_cost, int energy_cost, int energy_gain, Summon _summon, int _swapEffect, Status c_Status, Status p_Status,
            bool isprepare, Status prepareSkill)
        : Dmg(dmg), Element(element), Offensive(offensive), Healing(healing), Cleanse(cleanse), Defensive(defensive), 
          Dice_Cost(dice_cost), Energy_Cost(energy_cost), Energy_Gain(energy_gain), 
          summon(_summon), swapEffect(_swapEffect), C_Status(c_Status), P_Status(p_Status),
          IsPrepare(isprepare), PrepareSkill(prepareSkill) {}
};
struct Character     {
    string name; 
    int max_HP;       
    int HP;          
    int EGY; 
    string WeaponType;  

    Ability a;    
    Ability e;     
    Ability q;  
    Ability a2;
    Ability e2;

    vector<Status> moveQueue;
    StatusManager SM;
    vector<int> Inflicted_Element;
    bool Frozen;

    void gainEnergy(Ability x){
        if(x.Energy_Cost != 0){ //burst
            EGY = 0;
        }
        else {
            if (EGY < q.Energy_Cost) {
                EGY += x.Energy_Gain;
            }
        }
    }
    Character(){};
    Character(string _name, int _max_HP, int _HP, int _EGY, string weaponType, Ability _a, Ability _e, Ability _q)
        : name(_name), max_HP(_max_HP), HP(_HP), EGY(_EGY), WeaponType(weaponType), a(_a), e(_e), q(_q) {
            moveQueue = {};
            SM = StatusManager();
            Inflicted_Element = {};
            Frozen = false;
            a2.Dice_Cost = 100;
            e2.Dice_Cost = 100;
        }
    Character(string _name, int _max_HP, int _HP, int _EGY, string weaponType, Ability _a, Ability _e, Ability _q, Status passive)
        : name(_name), max_HP(_max_HP), HP(_HP), EGY(_EGY), WeaponType(weaponType), a(_a), e(_e), q(_q) {
            moveQueue = {};
            SM = StatusManager();
            Inflicted_Element = {};
            Frozen = false;
            a2.Dice_Cost = 100;
            e2.Dice_Cost = 100;
        }
    Character(string _name, int _max_HP, int _HP, int _EGY, string weaponType, Ability _a, Ability _e, Ability _q, Ability _a2, Ability _e2)
        : name(_name), max_HP(_max_HP), HP(_HP), EGY(_EGY), WeaponType(weaponType), a(_a), e(_e), q(_q), a2(_a2), e2(_e2) {
            moveQueue = {};
            SM = StatusManager();
            Inflicted_Element = {};
            Frozen = false;
        }
    Character(string _name, int _max_HP, int _HP, int _EGY, string weaponType, Ability _a, Ability _e, Ability _q, Ability _a2, Ability _e2, Status passive)
        : name(_name), max_HP(_max_HP), HP(_HP), EGY(_EGY), WeaponType(weaponType), a(_a), e(_e), q(_q), a2(_a2), e2(_e2) {
            moveQueue = {};   
            SM = StatusManager();
            SM.Statuses.push_back(passive);
            Inflicted_Element = {};
            Frozen = false;       
        }
};
struct Player        {
    string Name;
    array <Character, 3> Char;
    int Dice;
    int Active;
    bool End;
    int SwapCost = 1;
    SummonZone summon_Zone;
    StatusManager SM;

    vector<Card> Hand;

    int Left()  { return (( Active + 2) % 3); }
    int Right() { return (( Active + 1) % 3); }
    
    bool        Alive() { return Char[ Active].HP > 0; };
    bool    LeftAlive() { return Char[ Left()].HP > 0; };
    bool   RightAlive() { return Char[Right()].HP > 0; };
    void  SwappedLeft() {
        if      ( LeftAlive()){Active =  Left();}
        else if (RightAlive()){Active = Right();}
    }
    void SwappedRight() {
        if      (RightAlive()){Active = Right();}
        else if ( LeftAlive()){Active =  Left();}
    }
    static void  SwappedLeft(Player &D_P, Player &TempD_P) {
        if      ( TempD_P.LeftAlive()){D_P.Active =  TempD_P.Left();}
        else if (TempD_P.RightAlive()){D_P.Active = TempD_P.Right();}
    }
    static void SwappedRight(Player &D_P, Player &TempD_P) {
        if      (TempD_P.RightAlive()){D_P.Active = TempD_P.Right();}
        else if ( TempD_P.LeftAlive()){D_P.Active =  TempD_P.Left();}
    }
    bool checkHand(string cardName) {
        for(Card c : Hand) {
            if (cardName == c.Name) { return true; }
        }
        return false;
    }

    void updateQueue() {
        for(int i = 0; i<3; i++) {
            if(i != Active) {
                Char[i].moveQueue.clear();
                for(auto &s : Char[i].SM.Statuses) {
                    if(s.Name == "Tidecaller Shield") {
                        Char[i].SM.Usage_Update(s.Name, s.Usages);
                    }
                    if(s.Name == "Heron Shield") {
                        Char[i].SM.Usage_Update(s.Name, s.Usages);
                    }
                }
            }
        }
    };
    bool CheckFastSwap() {
        Character &A_Char = Char[Active];
        for (auto &c_s : A_Char.SM.Statuses) {
            string &acs = c_s.Name;
            if (acs == "Illusory Torrent") {
                if(c_s.Once_Per_Round) { 
                    c_s.Once_Per_Round = false;
                    c_s.Counter = 0;
                    return true;
                }
            }
        }  
        for(auto &s : summon_Zone.Summons) {
            if(s.Name == "Solar Isotoma") {
                if(s.Once_per_round) {
                    s.Once_per_round = false;
                    return true;
                }
            }
        }
        return false;
    }
    
    Player(string name, Character c1, Character c2, Character c3, int start) : Name(name), Char{c1, c2, c3}, Active(start - 1) {
        Dice = 8;
        End = false;
        summon_Zone = SummonZone();
        SM = StatusManager(); 
    }
};
// dmg(. < >), element(. < >), dice cost, energy cost, energy gain

#endif