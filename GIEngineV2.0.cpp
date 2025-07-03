#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include "CharacterList2.0.h"
using namespace std;


struct GameState;
struct Create {
    static void C_Summon       (GameState &state, Summon summon, SummonZone &zone);
    static void C_CharStatus   (GameState &state, Status c_s);
    static void C_PlayerStatus (GameState &state, Status p_s);
    static void C_PrepareSkill(GameState &state, Status prepare_skill);
};
void ResolveDeath        (GameState &state);
void applySkill          (GameState &state, int move, Ability &SummonAttack, int d_index);
GameState applyMove(const GameState &state, int move);

struct GameState {

    int Turn = 1;
    int Action = 1; 
    Player P1;
    Player P2;
    
    bool isP1Turn    = true;
    bool P1_EndFirst = false;

    Player *A_P;
    Player *D_P;

    Character *Attacker;
    Character *AttackerLeft;
    Character *AttackerRight;
    Character *Defender;
    Character *DefenderLeft;
    Character *DefenderRight;

    bool ALeftAlive;
    bool ARightAlive;
    bool DLeftAlive;
    bool DRightAlive;

    double Evaluate() { //P1 does better when eval is higher
        return ( P_HP(P1) -  P_HP(P2)
         + 0.1*(P_EGY(P1) - P_EGY(P2))
         + 0.1*(P1.Dice - P2.Dice));
    };
    void SetReferences() {
        A_P = (isP1Turn ? &P1 : &P2);
        D_P = (isP1Turn ? &P2 : &P1);
        
        Attacker      = &A_P->Char[A_P->Active ];
        AttackerLeft  = &A_P->Char[A_P->Left() ];
        AttackerRight = &A_P->Char[A_P->Right()];
        Defender      = &D_P->Char[D_P->Active ];
        DefenderLeft  = &D_P->Char[D_P->Left() ];
        DefenderRight = &D_P->Char[D_P->Right()];

        ALeftAlive  = A_P->LeftAlive();
        ARightAlive = A_P->RightAlive();
        DLeftAlive  = D_P->LeftAlive();
        DRightAlive = D_P->RightAlive();
    }
    
    void ApplyDmg      (array<int,3> Dmg,     bool to_Oppo) {
        if(to_Oppo) {
            if (Defender->HP > 0)     { Defender->HP      = max(0, Defender->HP      - Dmg[0]);}
            if (DefenderLeft->HP > 0) { DefenderLeft->HP  = max(0, DefenderLeft->HP  - Dmg[1]);}
            if (DefenderRight->HP > 0){ DefenderRight->HP = max(0, DefenderRight->HP - Dmg[2]);}
        }
        else {
            if (Attacker->HP > 0)     { Attacker->HP      = max(0, Attacker->HP      - Dmg[0]);}
            if (AttackerLeft->HP > 0) { AttackerLeft->HP  = max(0, AttackerLeft->HP  - Dmg[1]);}
            if (AttackerRight->HP > 0){ AttackerRight->HP = max(0, AttackerRight->HP - Dmg[2]);}
        }
    };
    void ApplyHealing  (array<int,3> Healing, bool to_Self) {
        if(to_Self) {
            if (Attacker->HP > 0)      { Attacker->HP      = min(Attacker->max_HP,      Attacker->HP      += Healing[0]);}
            if (AttackerLeft->HP > 0)  { AttackerLeft->HP  = min(AttackerLeft->max_HP,  AttackerLeft->HP  += Healing[1]);}
            if (AttackerRight->HP > 0) { AttackerRight->HP = min(AttackerRight->max_HP, AttackerRight->HP += Healing[2]);}
        }
    };

    int P_HP (Player &P) { 
        return P.Char[0].HP + P.Char[1].HP + P.Char[2].HP;  
    }
    int P_EGY(Player &P) { 
        return P.Char[0].EGY + P.Char[1].EGY + P.Char[2].EGY; 
    }
    
    bool Attacker_HP_Check() {return (Attacker->HP <= 0); }
    bool Defender_HP_Check() {return (Defender->HP <= 0); }
    
    void GameOver(GameState &finalState) {
        if (P_HP(P1) == 0) { cout << "P2 wins!" << endl; } 
        if (P_HP(P2) == 0) { cout << "P1 wins!" << endl; }
        exit(0);
    }

    GameState(Player p1, Player p2, bool _isP1Turn) : P1(p1), P2(p2), isP1Turn(_isP1Turn) {
        SetReferences();
    }
};
struct Trigger   {
    static void T_Summon                   (GameState &state, Summon &s, SummonZone &summonZone) {
        state.SetReferences();
        Ability SummonAttack = Ability(s.Dmg, s.Element, s.Offensive, s.Healing, s.Cleanse, s.Defensive);
        
        //special effects when triggering summons ep
        if(s.Name == "Jinni") {
            SummonAttack.Energy_Gain = 1;
        }
        summonZone.Usage_Update(state.A_P->SM, state.Attacker->SM, s.Name, 1);
        applySkill(state, -2, SummonAttack, 0);       
}
    
    static void T_A_Summon_Effects         (GameState &state, Ability &Attack, int &A_P_SwapCost, int move) {
        for (auto &s : state.A_P->summon_Zone.Summons) {
            if (s.Name == "Garden of Purity") {
                if (move == 3) {
                    Attack.Dmg[0] += 1;
                }
            }    
            if (s.Name == "Talisman Spirit")  {
                if (Attack.Element[0] == 1 || Attack.Element[0] == 8) {
                    Attack.Dmg[0] += 1;
                } 
            }              
            if (s.Name == "Solar Isotoma")    {
                if (state.Attacker->name == "Albedo" && move == 5) {
                    Attack.Dmg[0] += 2;
                }
            }
        }
    }
    static void T_A_Char_Status            (GameState &state, Ability &Attack, int &A_P_SwapCost, int move) {
        for (auto &c_s : state.Attacker->SM.Statuses) {
            string &acs = c_s.Name;
            //Usage Based
            if (acs == "Explosive Spark")       {
                if(state.A_P->Dice % 2 == 0) {
                    if(move == 3) {
                        Attack.Dice_Cost -= 1;
                        Attack.Dmg[0] += 1;
                        state.Attacker->SM.Usage_Update(acs, 1);
                    }
                    else if (move == -1) {
                        state.Attacker->a.Dice_Cost -= 1;
                        state.Attacker->SM.Usage_Update(acs, 1);
                    }

                }
            }
            if (acs == "Grimheart")             {
                if(move == 4) {
                    Attack.Dmg[0] += 3;
                }
            }
            if (acs == "Niwabi Enshou")         {
                if(move == 3) {
                    Attack.Dmg[0] += 1;
                    Attack.Element[0] = 3;
                    state.Attacker->SM.Usage_Update(acs, 1);
                }     
            }
            if (acs == "Takimeguri Kenka")      {
                if(move == 3) {
                    Attack.Dmg[0] +=1;
                    Attack.Element[0] = 2;
                    state.Attacker->SM.Usage_Update(acs, 1);
                }
            }
            if (acs == "Vijnana Suffusion")     {
                if(move == 3 && state.A_P->Dice %2 == 0) {
                    Attack.Element[0] = 7;
                    Attack.summon = Summon("Clusterbloom Arrow", 1, 2, {1,0,0},{7,0,0},true,{0,0,0},0,false);
                    state.Attacker->SM.Usage_Update(acs, 1);
                }
            }
            //Round Based
            if (acs == "Chisel-Light Mirror")   {
                if(move == 5) {
                    Attack.Dmg[0] += c_s.Rounds;
                    c_s.Rounds = 3 - c_s.Rounds;
                }
            }
            if (acs == "Lightfall+2")           {
                for(auto &s : state.A_P->summon_Zone.Summons) {
                    if(s.Name == "Lightfall Sword") {
                        s.Dmg[0] += 2;
                        Attack.Energy_Gain = 0;
                    }

                }
            }
            if (acs == "Pyro Infusion")         {
                if(move == 3) { Attack.Element[0] = 3; }
            }
            if (acs == "Sweeping Time")         {
                if(c_s.Once_Per_Round) {
                    if(move == 3) {
                        Attack.Dice_Cost -= 1;
                        Attack.Dmg[0] += 2;
                        Attack.Element[0] = 6;
                        c_s.Once_Per_Round = false;
                    }
                    else if (move == -1) {
                        state.Attacker->a.Dice_Cost -= 1;
                    }
                }
            }
            //Passive
            if (acs == "DilucE+2")              {
                if(move == 4) {
                    c_s.Counter += 1;
                    if(c_s.Counter == 3) {
                        Attack.Dmg[0] += 2;
                    }
                }
            }
            if (acs == "Herald_Hydro+1")        {
                if(move == 3 || move == 4 || move == 5 || move == -5) {
                    Attack.Element[0] = 2;
                    Attack.Dmg[0] += 1;

                }

            }
            if (acs == "Lightning Stilleto")    {
                if(move == 4 || !state.A_P->checkHand("Lightning Stilleto")){
                    state.A_P->Hand.push_back(Card("Lightning Stilleto"));
                }
            }
            if (acs == "Pactsworn Pathclearer") {
                if(c_s.Counter >= 2) { Attack.Element[0] = 4; }
                if(c_s.Counter >= 4) { Attack.Dmg[0] += 2;    }
                if(move == 4) { c_s.Counter += 1; }
                if(move == 5) { c_s.Counter += 2; }
            }
            if (acs == "Lector_Pyro+1")         {
                if(move == 3 || move == 4 || move == 5) {
                    Attack.Dmg[0] += 1;
                }

            }
            if (acs == "Radical Vitality")      {
                if (move == 5) {
                    Attack.Dmg[0] += c_s.Counter;
                    c_s.Counter = 0;
                }
                if (move == 3 || move == 4) {
                    if(Attack.Element[0] != 8 && Attack.Element[0] != 9 && Attack.Element[0] != 0) {
                        if(c_s.Counter < c_s.Limit) {
                            c_s.Counter += 1;
                        }
                    }
                }
            }
        }
    }
    static void T_A_Player_Status          (GameState &state, Ability &Attack, int &A_P_SwapCost, int move) {
        // move = -1: getValidMoves, move = -2 summon shoot, move = -3 fua, move = -6 Prepare Skill
            for (auto &p_s : state.A_P->SM.Statuses) {
                string &aps = p_s.Name;
                // Usage Based
                if (aps == "Bloom Core")                  {
                    if(Attack.Element[0] == 3 || Attack.Element[0] == 4) {
                        Attack.Dmg[0] += 2;
                        state.A_P->SM.Usage_Update(aps, 1);
                    }
                }
                if (aps == "Catalyzing Field")            {
                    if(Attack.Element[0] == 4 || Attack.Element[0] == 7) {
                        Attack.Dmg[0] += 1;
                        state.A_P->SM.Usage_Update(aps, 1);
                    }
                }
                if (aps == "Curse of the Undercurrent")   {
                    if(move == 4 || move == 5 || move == 7) {
                        Attack.Dice_Cost += 1;
                        state.A_P->SM.Usage_Update(aps, 1);
                    }
                    if(move == -1) {
                        state.Attacker->e.Dice_Cost += 1;
                        state.Attacker->q.Dice_Cost += 1;
                        state.Attacker->e2.Dice_Cost += 1;
                    }
                }
                if (aps == "Icy Quill")                   {
                    for (int i = 0; i < 3; ++i) {
                        if (Attack.Element[i] == 1) {
                            if (move == 3 || move == 4 || move == 5 || move == 6 || move == -5 || move == -6 || move == -7) {
                                Attack.Dmg[i] += 1;
                                state.A_P->SM.Usage_Update(aps, 1);
    
                                auto it = std::find_if(state.A_P->SM.Statuses.begin(), state.A_P->SM.Statuses.end(), [&](const Status& s) { return s.Name == aps; });
                                if (it == state.A_P->SM.Statuses.end()) { break; }
                            }
                        }
                    }
                }
                if (aps == "Illusory Bubble")             {
                    if(move == 3 || move == 4 || move == 5 || move == 6 || move == 7) {
                        Attack.Dmg[0] *= 2;
                        state.A_P->SM.Usage_Update(aps, 1);
                    }
                }
                // Round Based
                if (aps == "Chonghua Frost Field")        {
                    if (state.Attacker->WeaponType == "Sword" || state.Attacker->WeaponType == "Claymore" || state.Attacker->WeaponType == "Polearm") {
                        if((move == 3 || move == 4 || move == 5 || move == 6 || move == 7 | move == -5) && Attack.Element[0] == 8) {
                            Attack.Element[0] = 1;
                        }
                    } 
                }
                if (aps == "Inspiration Field")           {
                    if (move == 3 || move == 4 || move == 5 || move == 6 || move == 7 || move == -6) {
                        if (state.Attacker->HP >= 7) {
                            Attack.Dmg[0] += 2;
                        }
                    }
                }
                if (aps == "Prayer of the Crimson Crown") {
                    if (state.Attacker->WeaponType == "Sword" || state.Attacker->WeaponType == "Claymore" || state.Attacker->WeaponType == "Polearm") {
                        if((move == 3 || move == 4 || move == 5 || move == 6 || move == 7 | move == -5) && Attack.Element[0] == 8) {
                            Attack.Element[0] = 2;
                        }
                    } 
                    if (move == 3 || move == 6) {
                        Attack.Dmg[0] += 1;
                    }
                }
            }
        }           
    static void T_D_Summon_Effects         (GameState &state, Ability &Attack, int &A_P_SwapCost, int move) {
        for (auto &s : state.D_P->summon_Zone.Summons) {
            if (s.Name == "Baron Bunny Shield")        {
                if (Attack.Dmg[0] >= 1 && s.Effect_Usages >= 1) {
                    Attack.Dmg[0] -= 2;
                    s.Effect_Usages -= 1;
                }
            }
            if (s.Name == "Chains of Warding Thunder") {
                if(move == 1 || move == 2 || move == -1) {
                    if(s.Effect_Active) {
                        state.A_P->SwapCost += 1;
                        s.Effect_Active = false;
                    }
                }
            } 
            if (s.Name == "Reflection")                {
                if (Attack.Dmg[0] >= 1 && s.Effect_Usages >= 1) {
                    Attack.Dmg[0] -= 1;
                    s.Effect_Usages -= 1;
                }
            }
        }
    }
    static void T_D_Char_Status            (GameState &state, Ability &Attack, int &A_P_SwapCost, int move, int d_index) {
        Character dummyD; 
        Character *D_Char = &dummyD;  
        if      (d_index == 0) { D_Char = state.Defender;      }
        else if (d_index == 1) { D_Char = state.DefenderLeft;  }
        else if (d_index == 2) { D_Char = state.DefenderRight; }
        for (auto &c_s : D_Char->SM.Statuses) {
            string &dcs = c_s.Name;
            if(dcs == "Radical Vitality")  {
                if (Attack.Element[d_index] != 8 && Attack.Element[d_index] != 9 && Attack.Element[d_index] != 0) {
                    if(c_s.Counter < c_s.Limit) {
                        c_s.Counter += 1;
                    }
                }
            }
            if(dcs == "Tidecaller Shield") {
                for(int i = 0; i <3; i++) {
                    if(d_index == 0) {
                        int shieldConsumed = min(c_s.Usages, Attack.Dmg[0]);
                        Attack.Dmg[0] -= shieldConsumed; 
                        state.Defender->SM.Usage_Update(dcs, shieldConsumed);
                        
                    }
                }
            }
            if(dcs == "Heron Shield")      {
                for(int i = 0; i <3; i++) {
                    if(d_index == 0) {
                        int shieldConsumed = min(c_s.Usages, Attack.Dmg[0]);
                        Attack.Dmg[0] -= shieldConsumed; 
                        state.Defender->SM.Usage_Update(dcs, shieldConsumed);
                        
                    }
                }
            }
        }
    } 
    static void T_D_Player_Status          (GameState &state, Ability &Attack, int &A_P_SwapCost, int move, int d_index) {        
        if(d_index == 0) {
            for (auto &p_s : state.D_P->SM.Statuses) {
                string &dps = p_s.Name;
                if (dps == "Full Plate")           { //halve last?
                    if (Attack.Element[0] == 8) { Attack.Dmg[0] = (Attack.Dmg[0] + 1) / 2; }
                    int shieldConsumed = min(p_s.Usages, Attack.Dmg[0]);
                    Attack.Dmg[0] -= shieldConsumed; 
                    state.D_P->SM.Usage_Update(dps, shieldConsumed);
                }
                if (dps == "Cryst_Shield")         {
                    int shieldConsumed = min(p_s.Usages, Attack.Dmg[0]);
                    Attack.Dmg[0] -= shieldConsumed; 
                    state.D_P->SM.Usage_Update(dps, shieldConsumed);
                }
                if (dps == "Cat-Claw Shield")      {
                    int shieldConsumed = min(p_s.Usages, Attack.Dmg[0]);
                    Attack.Dmg[0] -= shieldConsumed; 
                    state.D_P->SM.Usage_Update(dps, shieldConsumed);
                }
                if (dps == "Shield of Passion")    {
                    int shieldConsumed = min(p_s.Usages, Attack.Dmg[0]);
                    Attack.Dmg[0] -= shieldConsumed; 
                    state.D_P->SM.Usage_Update(dps, shieldConsumed);
    
                }
                if (dps == "Rain Sword")           {
                    if (Attack.Dmg[0] >= 3) {
                        Attack.Dmg[0] -= 1;
                        state.D_P->SM.Usage_Update(dps, 1);
                    }
                }
                if (dps == "Thunderbeast's Targe") {
                    if (Attack.Dmg[0] >= 3) {
                        Attack.Dmg[0] -= 1;
                    }
                }     
                if (dps == "Ice Lotus")            {
                    if (Attack.Dmg[0] >= 2) {
                        Attack.Dmg[0] -= 1;
                        state.D_P->SM.Usage_Update(dps, 1);
                    }
                } 
            }
        }
    }      
    
    static void T_Player_Revive_Status     (GameState &state, Player &P) {
        for(Character &c : P.Char) {
            if(c.HP == 0) {
                for (auto &c_s : c.SM.Statuses) {
                    string &cs = c_s.Name;
                    if(cs == "Electro Crystal Core") {
                        if(c_s.Counter == 1) {
                            c_s.Counter -= 1;
                            c.HP = 1;
                        }
                    }
                    if(cs == "Watery Rebirth") {
                        if(c_s.Counter == 1) {
                            c_s.Counter -= 1;
                            c.HP = 4;
                            Create::C_CharStatus(state, {"Herald_Hydro+1",1,1,false,false,false,true});
                        }
                    }
                    if(cs == "Fiery Rebirth") {
                        if(c_s.Counter == 1) {
                            c_s.Counter -= 1;
                            c.HP = 4;
                            Create::C_CharStatus(state, {"Lector_Pyro+1",1,1,false,false,false,true});
                        }
                    }
                
                }
            }   
        }
    }
    static void T_Summon_Effect_onCreation (GameState &state, Summon &summon, SummonZone &summonZone) {
        for(auto &s: summonZone.Summons) {
            if(s.Name == summon.Name) {           
                if (summon.Name == "Sesshou Sakura") {
                    if(s.Duration >= 4) {
                        s.Dmg[0] = 2;
                    }
                }
                if (summon.Name == "Reflection") {
                    s.Effect_Usages = 1;
                }
                if (summon.Name == "Baron Bunny Shield") {
                    s.Effect_Usages = 1;
                }
                if (summon.Name == "Chains of Warding Thunder") {
                    s.Effect_Active = true;
                }
            }
        }          
    }

    static void T_Char_Status_onCreation   (GameState &state, Status &c_Status) {
        string cName = c_Status.Name;
        if(cName == "Grimheart")     {
            if (c_Status.Usages == 2) {
                state.Attacker->SM.Usage_Update(cName, 2);
            }
        }
        if(cName == "Sweeping Time") {
            c_Status.Once_Per_Round = true;
        }
    }
    static void T_Player_Status_onCreation (GameState &state, Status &p_Status) {
        string &pName = p_Status.Name;
        if(pName == "Inspiration Field") {
            if (state.Attacker->HP < 7) {
                state.Attacker->HP = min(state.Attacker->max_HP, state.Attacker->HP += 2);
            }
        }
        if(pName == "Scorching Ooyoroi") {
            Create::C_PlayerStatus(state, Status("Blazing Barrier", true, 1, 3));
        }
        if(pName == "Tenko Check")       {
            for(auto &s : state.A_P->summon_Zone.Summons) {
                if(s.Name == "Sesshou Sakura") {
                    state.A_P->summon_Zone.Usage_Update(state.A_P->SM, state.Attacker->SM, s.Name, s.Duration);
                    state.A_P->SM.Statuses.push_back(Status("Tenko Thunderbolts", true, 1, 1, false, false, false));
                }
            }
            state.A_P->SM.Usage_Update(pName, 1);
        }
    }
    static void T_Char_Status_fua          (GameState &state, Status &cfu_status,    int move) {
        state.SetReferences();
        string cfu = cfu_status.Name;
        Ability fua = Ability({0,0,0},{0,0,0},false,{0,0,0},0,false);
        // Round Based
        if (cfu == "Chisel-Light Mirror") {
            if (move == 3) {
                fua.Dmg     = {1, 0, 0};
                fua.Element = {7, 0, 0};
                fua.Offensive = true;
                applySkill(state, -3, fua,0); 
                if (state.A_P->Dice %2 == 0 && cfu_status.Rounds < cfu_status.Limit) {
                    cfu_status.Rounds += 1;
                }
            }
        }
        if (cfu == "The Wolf Within")     {
            if(move == 3 || move == 4) {
                fua.Dmg     = {2, 0, 0};
                fua.Element = {4, 0, 0};
                fua.Offensive = true;
                applySkill(state, -3, fua,0);
            }
        }
    }
    static void T_Player_Status_fua        (GameState &state, Status &pfu_status,    int move) {   
        state.SetReferences();
        string pfu_name = pfu_status.Name;
        Ability fua = Ability({0,0,0},{0,0,0},false,{0,0,0},0,false);  
        // Usage Based
        if      (pfu_name == "Icicle")              {
            if (move == 1 || move == 2 || move == 8 || move == 9) {
                fua.Dmg     = {2, 0, 0};
                fua.Element = {1, 0, 0};
                fua.Offensive = true;
                state.A_P->SM.Usage_Update(pfu_name, 1); 
                applySkill(state, -3, fua,0);
            }
        }
        else if (pfu_name == "Pyronado")            {
            if (move == 3 || move == 4 || move == 5 || move == 6 || move == 7) {
                fua.Dmg     = {2, 0, 0};
                fua.Element = {3, 0, 0};
                fua.Offensive = true;
                state.A_P->SM.Usage_Update(pfu_name, 1); 
                applySkill(state, -3, fua,0);
            }
        }
        else if (pfu_name == "Rainbow Bladework")   {
            if (move == 3 || move == 6) {
                fua.Dmg     = {1,0,0};
                fua.Element = {2,0,0};
                fua.Offensive = true;
                state.A_P->SM.Usage_Update(pfu_name, 1); 
                applySkill(state, -3, fua,0);
            }
        } 
        else if (pfu_name == "Scorching Ooyoroi")   {
            if (move == 3 || move == 6) {
                fua.Dmg     = {1, 0, 0};
                fua.Element = {3, 0, 0};
                fua.Offensive = true;
                applySkill(state, -3, fua,0);
                Create::C_PlayerStatus(state, Status("Blazing Barrier",true,1,3));
            }
        }
        else if (pfu_name == "Sparks 'n' Splash")   {
            if (move == 3 || move == 4 || move == 5 || move == 6 || move == 7) {
                fua.Dmg     = {2, 0, 0};
                fua.Element = {3, 0, 0};
                fua.Offensive = true;
                fua.self = true;
                state.A_P->SM.Usage_Update(pfu_name, 1); 
                applySkill(state, -3, fua,0);
            }
        }
        // Round Based
        else if (pfu_name == "Aurous Blaze")        {
            if (state.Attacker->name != "Yoimiya" && (move == 3 || move == 4 || move == 5 || move == 6 || move == 7)) {
                fua.Dmg     = {1, 0, 0};
                fua.Element = {3, 0, 0}; 
                fua.Offensive = true;
                applySkill(state, -3, fua,0);
            }
        } 
        else if (pfu_name == "Inspiration Field")   {
            if (move == 3 || move == 4 || move == 5 || move == 6 || move == 7) {
                if (state.Attacker->HP < 7) { state.Attacker->HP = min(state.Attacker->max_HP, state.Attacker->HP + 2); }
            }
        } 
        else if (pfu_name == "Thunderbeast's Targe"){
            if (move == 3 || move == 6) {
                fua.Dmg     = {1, 0, 0};
                fua.Element = {4, 0, 0}; 
                fua.Offensive = true;
                applySkill(state, -3, fua,0);
            }
        } 
        else if (pfu_name == "Prayer of the Crimson Crown") {
            if(move == 1 || move == 2 || move == 8 || move == 9) {
                if (pfu_status.Once_Per_Round) {
                    pfu_status.Once_Per_Round = false;
                    fua.Dmg     = {1, 0, 0};
                    fua.Element = {2, 0, 0}; 
                    fua.Offensive = true;
                    applySkill(state, -3, fua,0);             
                }
            }
        }
    } 
    static void T_Player_Status_PreAtk     (GameState &state, Status &preatk_status, int move) {
        string baps = preatk_status.Name;
        Ability preAtk = Ability({0,0,0},{0,0,0},false,{0,0,0},0,false); 
        if(baps == "Tenko Thunderbolts") {
            preAtk.Dmg     = {3,0,0};
            preAtk.Element = {4,0,0};
            state.A_P->SM.Usage_Update(baps, 1); 
            applySkill(state, -3, preAtk,0);
        }
    }
    
    static void T_Summon_On_End_Effects         (GameState &state) {
        for(auto &summon : state.A_P->summon_Zone.Summons) {
            string sname = summon.Name;
            if(summon.Name == "Sesshou Sakura") {
                if(summon.Duration >= 4) {
                    Trigger::T_Summon(state, summon, state.A_P->summon_Zone);
                }
            }
        }
    }
    //static void T_Char_On_End_Effects()
    static void T_Player_On_End_Effects         (GameState &state) {
        for (auto &s : state.A_P->SM.Statuses) {
            string &p_oes = s.Name;
            // Usage Based
            if(p_oes == "Festive Fires") { 
                if(state.A_P->Hand.size() <= 1) {
                    Ability FestiveFire = Ability({1,0,0},{3,0,0},true,{0,0,0},0,false);
                    applySkill(state, -3, FestiveFire,0);
                    state.A_P->SM.Usage_Update(p_oes, 1);
                }
            }
        }
    }
    static void T_Player_EndTurn_Status_Effects (GameState &state) {  
        for(int i = state.A_P->Active; i <state.A_P->Active+3; i++) { //i goes 0 1 2, 1 2 3, 2 3 4
            int j = i%3; // j goes 0 1 2, 1 2 0, 2 0 1
            for(auto &s: state.A_P->Char[j].SM.Statuses) {
                if (s.Name == "Snappy Silhouette") {
                    int d_index = (state.A_P->Active - j + 3)%3; 
                    Ability fua = {{0,0,0},{0,0,0},true,{0,0,0},0,false};
                    fua.Element[d_index] = 1;
                    fua.Dmg[d_index] = 1; 
                    fua.self = true;
                    if (s.Usages == 1 && std::find(state.A_P->Char[j].Inflicted_Element.begin(), state.A_P->Char[j].Inflicted_Element.end(), 1) 
                                                != state.A_P->Char[j].Inflicted_Element.end()) {
                        fua.Dmg[d_index] = 2; 
                    }
                    applySkill(state, -3, fua, d_index); // apply to 
                    state.A_P->Char[j].SM.Usage_Update(s.Name,1);
                }
                state.A_P->Char[j].SM.Round_Update();
            }
        }
        
        for(auto &s: state.A_P->SM.Statuses) {
            //if ??
            state.A_P->SM.Round_Update();
        }
    }
    static Ability Find_Prepare_Skill      (GameState &state) {
        Ability PrepareSkill = Ability();
        string PrepareName = state.Attacker->moveQueue[0].Name;
        state.Attacker->moveQueue.pop_back();
        if(PrepareName == "Wavestrider") {
            for(auto &cs : state.Attacker->SM.Statuses) {
                if(cs.Name == "Tidecaller Shield") {
                    state.Attacker->SM.Usage_Update(cs.Name, cs.Usages);
                }
            }
            PrepareSkill.Dmg = {3,0,0};
            PrepareSkill.Element = {4,0,0};
            PrepareSkill.Offensive = true;
        }
        if(PrepareName == "Heron Strike") {
            for(auto &cs : state.Attacker->SM.Statuses) {
                if(cs.Name == "Heron Shield") {
                    state.Attacker->SM.Usage_Update(cs.Name, cs.Usages);
                }
            }
            PrepareSkill.Dmg = {3,0,0};
            PrepareSkill.Element = {2,0,0};
            PrepareSkill.Offensive = true;
        }
        
        if(PrepareName == "Rippling Blades") {
            PrepareSkill.Dmg = {1,0,0};
            PrepareSkill.Element = {2,0,0};
            PrepareSkill.Offensive = true;
        }
        
        if(PrepareName == "RPS Scissors") {
            PrepareSkill.Dmg = {2,0,0};
            PrepareSkill.Element = {4,0,0};
            PrepareSkill.Offensive = true;
            Create::C_PrepareSkill(state, Status("RPS Paper"));
        }
        if(PrepareName == "RPS Paper")    {
            PrepareSkill.Dmg = {3,0,0};
            PrepareSkill.Element = {4,0,0};
            PrepareSkill.Offensive = true; 
        }
        return PrepareSkill;
    }
    
    static void T_All_Atk_Status_and_Effects(GameState &state, Ability &Attack, int A_P_Swap_Cost, int move) {
        T_A_Summon_Effects(state, Attack, A_P_Swap_Cost, move);
        T_A_Char_Status   (state, Attack, A_P_Swap_Cost, move);
        T_A_Player_Status (state, Attack, A_P_Swap_Cost, move);
    }
    static void T_All_Def_Status_and_Effects(GameState &state, Ability &Attack, int A_P_Swap_Cost, int move, int d_index) {
        T_D_Summon_Effects(state, Attack, A_P_Swap_Cost, move);
        T_D_Char_Status   (state, Attack, A_P_Swap_Cost, move, d_index);  
        T_D_Player_Status (state, Attack, A_P_Swap_Cost, move, d_index);
    }

    static vector<int> getValidMoves(const GameState &state) {
        vector<int> validMoves;
        GameState temp_state = state;
        temp_state.SetReferences();
        Ability dummy_Atk = Ability({0,0,0},{0,0,0},false,{0,0,0},0,false);
        int dummy_Swap_Cost = 1;
        Trigger::T_All_Atk_Status_and_Effects(temp_state, dummy_Atk, dummy_Swap_Cost, -1); //assume d is 0
        Trigger::T_All_Def_Status_and_Effects(temp_state, dummy_Atk, dummy_Swap_Cost, -1, 0);
            
        
        if (temp_state.A_P->Alive()) { 
            if(!temp_state.Attacker->moveQueue.empty() && !temp_state.Attacker->Frozen) {
                validMoves.push_back(-5);
            }
            else {
                validMoves.push_back(0);
                if (temp_state.A_P->Dice >= 1 &&  temp_state.AttackerLeft->HP > 0) {validMoves.push_back(1); }
                if (temp_state.A_P->Dice >= 1 && temp_state.AttackerRight->HP > 0) {validMoves.push_back(2); }
                if (temp_state.Attacker->Frozen == false) {
                    if (temp_state.A_P->Dice >= temp_state.Attacker->a.Dice_Cost)  { validMoves.push_back(3); }
                    if (temp_state.A_P->Dice >= temp_state.Attacker->e.Dice_Cost)  { validMoves.push_back(4); }
                    if (temp_state.A_P->Dice >= temp_state.Attacker->q.Dice_Cost && 
                        temp_state.Attacker->EGY >= temp_state.Attacker->q.Energy_Cost){ validMoves.push_back(5); }
                    if (temp_state.A_P->Dice >= temp_state.Attacker->a2.Dice_Cost) { validMoves.push_back(6); }
                    if (temp_state.A_P->Dice >= temp_state.Attacker->e2.Dice_Cost) { validMoves.push_back(7); }
                }
            }
        }
        else {
            if(temp_state.ALeftAlive)  {validMoves.push_back(8);}
            if(temp_state.ARightAlive) {validMoves.push_back(9);}
        }
        return validMoves;
    }
};

void Create::C_Summon       (GameState &state, Summon summon, SummonZone &zone) {
    if(summon.Name == "No Summon") { return; }
    bool added = false;
    for (auto &s : zone.Summons) {
        if (s.Name == summon.Name) {
            s.Duration = min(s.MaxDuration, s.Duration + summon.Duration);
            added = true;
            break;  
        }
    }
    if (!added && zone.Summons.size() < 4) {
        zone.Summons.push_back(summon);
        added = true; 
    }
    Trigger::T_Summon_Effect_onCreation(state, summon, zone);

}
void Create::C_CharStatus   (GameState &state, Status c_s) {
    Character &C = (c_s.onOpponentSide ? *state.Defender : *state.Attacker);        
    for (auto &c_status : C.SM.Statuses) {
        if (c_status.Name == c_s.Name) { 
            if(c_s.Usage_Based) { 
                c_status.Usages = min(c_s.Limit, c_status.Usages + c_s.Usages); 
            } else { 
                c_status.Rounds = min(c_s.Limit, c_status.Rounds + c_s.Rounds); 
            }
            if(c_s.TriggerOnCreate) { Trigger::T_Char_Status_onCreation(state, c_status); }
            return;
        }
    }
    C.SM.Statuses.push_back(c_s); 
    if (c_s.TriggerOnCreate) { Trigger::T_Char_Status_onCreation(state, c_s); }
}
void Create::C_PlayerStatus (GameState &state, Status p_s) {
    Player &P = (p_s.onOpponentSide ? *state.D_P : *state.A_P);
    for (auto &p_status : P.SM.Statuses) {
        if (p_status.Name == p_s.Name) { 
            if(p_s.Usage_Based) { 
                p_status.Usages = min(p_s.Limit, p_status.Usages + p_s.Usages); 
            } else { 
                p_status.Rounds = min(p_s.Limit, p_status.Rounds + p_s.Rounds); 
            }
            if(p_s.TriggerOnCreate) { Trigger::T_Player_Status_onCreation(state, p_status); }
            return;
        }
    }
    P.SM.Statuses.push_back(p_s); 
    if (p_s.TriggerOnCreate) { Trigger::T_Player_Status_onCreation(state, p_s); }
}
void Create::C_PrepareSkill (GameState &state, Status prepare_skill) {
    state.Attacker->moveQueue.push_back(prepare_skill);
}

struct ELogic    {
    static void ApplyElement    (GameState &state, array<int,3> &Dmg, array<int,3> &Element, Player &P, int d_index) {
        Character dummyD; 
        Character *Defender = &dummyD; 

        if      (d_index == 0) { Defender = &P.Char[ P.Active]; }
        else if (d_index == 1) { Defender = &P.Char[ P.Left()]; }
        else if (d_index == 2) { Defender = &P.Char[P.Right()]; }
        if (Defender->HP > 0) {
            if (Element[d_index] != 8 && Element[d_index] != 9 && Element[d_index] != 0) {
                Defender->Inflicted_Element.push_back(Element[d_index]);
            }
            if (Defender->Frozen && (Element[d_index] == 3 || Element[d_index] == 8)) {
                Dmg[d_index] += 2;
                Defender->Frozen = false;
            }
        }
    }
    static void CleanseElement  (GameState &state, array<int,3> &Healing,       int Cleanse, int d_index) {
        //d_index = 0 assumed for now
        if (state.Attacker->HP > 0) {
            if(Cleanse != 8 && Cleanse != 9 && Cleanse != 0) {
                state.Attacker->Inflicted_Element.push_back(Cleanse);
            }
            if(state.Attacker->Frozen && (Cleanse == 3 || Cleanse == 8)) {
                if(Healing[d_index] < 0) { //if manage to deal phys / pyro cleanse to self while frozen (idk crane + klee burst) / or tada ig
                    Healing[d_index] -= 2;
                    state.Attacker->Frozen = false;
                }
            }
        }
    };
    static string CheckReaction (GameState &state, Player &P, int d_index) {
        Character dummyD; 
        Character *Defender = &dummyD;  
        if      (d_index == 0) { Defender = &P.Char[ P.Active]; }
        else if (d_index == 1) { Defender = &P.Char[ P.Left()]; }
        else if (d_index == 2) { Defender = &P.Char[P.Right()]; }
        //Element(0-9): None C H P E G A D Phys Pierc
        vector<int> &Element = Defender->Inflicted_Element;
        if (Element.size() == 1) {
            if (Element[0] == 5 || Element[0] == 6) {
                Element.pop_back();
            }
        }
        else if (Element.size() == 2) {
            if(Element[0] == Element[1]) { //check duplicate, remove if duplicate
                Element.pop_back();
            }
            else {
                sort(Element.begin(), Element.end());
                if (Element == vector<int>{1,2}) {
                    Element.clear();
                    return string("Freeze");
                }
                else if (Element == vector<int> {1,3}) {
                    Element.clear();
                    return string("Melt");
                }
                else if (Element == vector<int> {1,4}) {
                    Element.clear();
                    return string("SC");
                }
                else if (Element == vector<int> {2,3}) {
                    Element.clear();
                    return string("Vaporize");
                }
                else if (Element == vector<int> {2,4}) {
                    Element.clear();
                    return string("EC");
                }
                else if (Element == vector<int> {3,4}) {
                    Element.clear();
                    return string("Overload");
                }
                else if (Element == vector<int> {2,7}) {
                    Element.clear();
                    return string("Bloom");
                }
                else if (Element == vector<int> {3,7}) {
                    Element.clear();
                    return string("Burning");
                }
                else if (Element == vector<int> {4,7}) {
                    Element.clear();
                    return string("Catalyzing Field");
                }
                else if (Element[1] == 5) {
                    int type = Element[0];
                    Element.clear();
                    return ("Swirl " + to_string(type));
                }
                else if (Element[1] == 6) {
                    int type = Element[0];
                    Element.clear();
                    return string("Crystallize");
                }
                else if (Element == vector<int> {1,7}) {
                    return string("fridge");
                }
                else if (Element == vector<int> {5,7}) {
                    Element = {7};
                }
                else if (Element == vector<int> {6,7}) {
                    Element = {7};
                }
            }
        }
        else if (Element.size() == 3) {
            if(Element[0] == Element[2] || Element[1] == Element[2]) { //cryo/dendro 3rd
                Element.pop_back();
            }
            else { // cryo dendro + 2,3,4,5,6
                if(Element[2] == 2){
                    Element = {7};
                    return string("Freeze");
                }
                if(Element[2] == 3){
                    Element = {7};
                    return string("Melt");
                }
                if(Element[2] == 4){
                    Element = {7};
                    return string("SC");
                }
                if(Element[2] == 5){
                    Element = {7};
                    return string("Swirl 1");
                }
                if(Element[2] == 6){
                    Element = {7};
                    return string("Crystallize");
                }
            }
        }
        return string("No Reaction"); 
    };
    static void ApplyReaction   (GameState state, Player &A_P, Player &D_P, Ability &Attack, string ReactionType, int d_index, bool Dmg) {
        Character dummyD; 
        Character *Defender = &dummyD;  
        if      (d_index == 0) { Defender = &D_P.Char[ D_P.Active]; } //allows flexibility to input D_P, A_P for clense burning
        else if (d_index == 1) { Defender = &D_P.Char[ D_P.Left()]; }
        else if (d_index == 2) { Defender = &D_P.Char[D_P.Right()]; }
        if(Dmg){
            if (ReactionType == "Vaporize" || ReactionType == "Melt") { Attack.Dmg[d_index] += 2; }
            if (ReactionType == "Overload"                          ) { Attack.Dmg[d_index] += 2; }
            if (ReactionType == "EC"       || ReactionType == "SC"  ) { for (int &D : Attack.Dmg) { D += 1; }}
            if (ReactionType == "Freeze"                            ) { Attack.Dmg[d_index] += 1; }
            if (ReactionType == "Bloom"                             ) { Attack.Dmg[d_index] += 1; }
            if (ReactionType == "Burning"                           ) { Attack.Dmg[d_index] += 1; }
            if (ReactionType == "Catalyzing Field"                  ) { Attack.Dmg[d_index] += 1; }
            if (ReactionType == "Crystallize"                       ) { Attack.Dmg[d_index] += 1; }
        } else {
            if (ReactionType == "Catalyzing Field") { Create::C_PlayerStatus(state, Status("Catalyzing Field", true, 2, 2)); }
            if (ReactionType == "Bloom"           ) { Create::C_PlayerStatus(state ,Status("Bloom Core",       true, 1, 1)); }
            if (ReactionType == "Crystallize"     ) { Create::C_PlayerStatus(state, Status("Cryst_Shield",     true, 1, 2)); }
            if (ReactionType == "Burning"         ) { Create::C_Summon(state, Summon("Burning",1,2,{1,0,0},{3,0,0},true,{0,0,0},0,false), A_P.summon_Zone); }
            if (ReactionType == "Freeze"          ) { Defender->Frozen = true; }    
            if (ReactionType == "Overload"        ) { 
                if(d_index == 0) {
                    D_P.SwappedRight(); 
                    D_P.updateQueue();
                } 
            }
            if (ReactionType.find("Swirl") != std::string::npos) { 
                int value = std::stoi(ReactionType.substr(6));  
                Ability swirl1 = Ability({0,1,0},{0,value,0},true,{0,0,0},0,false);
                Ability swirl2 = Ability({0,0,1},{0,0,value},true,{0,0,0},0,false);
                applySkill(state, -6, swirl1,1);
                applySkill(state, -7, swirl2,2);
            }
        }
    }
};
struct Printing  {
    static void printInflictedElement (int element)     {
        switch (element) {
            case 1: cout << LIGHT_BLUE  << "C "; break;   
            case 2: cout << DARK_BLUE   << "H "; break;   
            case 3: cout << RED         << "P "; break;          
            case 4: cout << PURPLE      << "E "; break;      
            case 5: cout << DARK_GREEN  << "A "; break;
            case 6: cout << YELLOW      << "G "; break; 
            case 7: cout << LIGHT_GREEN << "D "; break;  
        }
        cout << RESET;
    }
    static void PrintGameState (GameState state)        {
        cout << "\n--- Turn " << state.Turn << " Action " << state.Action << " ---\n\n" ;
        for(int i = 0; i < 3; i++) {
            if (!state.P2.Char[i].moveQueue.empty()) {
                cout << "Queue: " << state.P2.Char[i].moveQueue[0].Name << endl;
            }
        }
        state.P2.SM.printAllStatus("PS: ");
        cout << endl;
        for (int i = 0; i < 3; ++i) {
            state.P2.Char[i].SM.printAllStatus("CS" + to_string(i + 1) + ": ");
        }
        cout << "\n\n";
        //P2
        for (int i = 0; i < 3; ++i) {
            if (state.P2.Char[i].Frozen) { cout << LIGHT_BLUE << state.P2.Char[i].name << RESET << " "; } 
            else { cout << state.P2.Char[i].name; }
            cout <<"\t\t";
        } cout << endl;
        for (int i = 0; i < 3; i++) {
            for (int e : state.P2.Char[i].Inflicted_Element) { printInflictedElement(e); }
            cout <<"\t\t";
        } 
        cout << " | Dice " << state.P2.Dice << " | ";
        state.P2.summon_Zone.printSummons();
        cout << endl;
        for (int i = 0; i < 3; ++i) {
            if (i == state.P2.Active) { cout << BOLD << RED << state.P2.Char[i].HP << "(" << state.P2.Char[i].EGY << ")" << RESET << " "; } 
            else { cout << state.P2.Char[i].HP << "(" << state.P2.Char[i].EGY << ") "; }
            cout <<"\t\t";
        }
        cout << "\n\n\t\t\t\t\t\t\t\t Eval: " << state.Evaluate() << "\n";
        //P1
        for (int i = 0; i < 3; i++) {
            for (int e : state.P1.Char[i].Inflicted_Element) { printInflictedElement(e); }
            cout <<"\t\t";
        } cout << endl;
        for (int i = 0; i < 3; ++i) {
            if (i == state.P1.Active) { cout << BOLD << GREEN << state.P1.Char[i].HP << "(" << state.P1.Char[i].EGY << ")" << RESET << " "; } 
            else                      { cout << state.P1.Char[i].HP << "(" << state.P1.Char[i].EGY << ") "; }
            cout <<"\t\t";
        } 
        cout << " | Dice " << state.P1.Dice << " | ";
        state.P1.summon_Zone.printSummons();
        cout << endl;
        for (int i = 0; i < 3; ++i) {
            if (state.P1.Char[i].Frozen) { cout << LIGHT_BLUE << state.P1.Char[i].name << RESET << " "; } 
            else { cout << state.P1.Char[i].name; }
            cout <<"\t\t";
        }
        cout << "\n\n";
        for (int i = 0; i < 3; ++i) {
            state.P1.Char[i].SM.printAllStatus("CS" + to_string(i + 1) + ": ");
        }
        cout << endl;
        state.P1.SM.printAllStatus("PS: ");
        cout << "\n";
        for(int i = 0; i < 3; i++) {
            if (!state.P1.Char[i].moveQueue.empty()) {
                cout << "Queue: " << state.P1.Char[i].moveQueue[0].Name;
            }
        }
        cout << "\n\n";
        cout << (state.isP1Turn ? LIGHT_GREEN : RED) << (state.isP1Turn ? "P1" : "P2") << " Turn " << RESET;
    }
    static void PrintValidMoves(vector<int> validMoves) {
        cout << "Valid Moves:" << endl;
        for (int move : validMoves) {
            switch (move) {
                case 0: cout << "0: End Turn "             << endl; break;
                case 1: cout << "1: Swap Left "            << endl; break;
                case 2: cout << "2: Swap Right "           << endl; break;
                case 3: cout << "3: NA "                   << endl; break;
                case 4: cout << "4: E "                    << endl; break;
                case 5: cout << "5: Q "                    << endl; break;
                case 6: cout << "6: A2"                    << endl; break;
                case 7: cout << "7: E2"                    << endl; break;
                case 8: cout << "8: Choose Left "          << endl; break;
                case 9: cout << "9: Choose Right "         << endl; break;
                case-5: cout << "-5: Cont Prepare Skill "  << endl; break;   
            }
        }
        cout << endl;
    }
    static int AskMove() {
        int choice;
        cout << "Choose action (Enter move number): ";
        cin >> choice;
        return choice;
    }
    static int DisplayState_ShowValid_Ask(GameState state, vector<int> validMoves) {
        PrintGameState(state);
        PrintValidMoves(validMoves);
        int choice = AskMove();
        return choice;
    }
};


//This is for the Engine that determines the optimal moves to a certain depth
int minimax(GameState state, int depth, vector<int> validMoves, bool is_maximizing) {
    if (depth == 0 || validMoves.empty()) {
        return state.Evaluate(); 
    }

    int bestMove = -1;

    if (is_maximizing) {
        int maxEval = INT_MIN;
        // Loop through all valid moves to maximize the score
        for (int move : validMoves) {
            GameState newState = applyMove(state, move); // Apply the move
            int eval = minimax(newState, depth - 1, Trigger::getValidMoves(newState), false); // Recurse for the next move
            if (eval > maxEval) {
                maxEval = eval;
                bestMove = move; // Store the best move so far
            }
        }
        return bestMove; // Return the move that maximizes the score
    } else {
        int minEval = INT_MAX;
        // Loop through all valid moves to minimize the score
        for (int move : validMoves) {
            GameState newState = applyMove(state, move); // Apply the move
            int eval = minimax(newState, depth - 1, Trigger::getValidMoves(newState), true); // Recurse for the next move
            if (eval < minEval) {
                minEval = eval;
                bestMove = move; // Store the best move so far
            }
        }
        return bestMove; // Return the move that minimizes the score
    }
}

void ResolveDeath (GameState &state) { 
    vector<int> validMoves;
    
    Trigger::T_Player_Revive_Status(state, *state.A_P);
    Trigger::T_Player_Revive_Status(state, *state.D_P);

    bool currentTurn = state.isP1Turn;
    while (state.Defender_HP_Check() || state.Attacker_HP_Check()) {
        if (state.Defender_HP_Check()) {  
            state.Defender->Inflicted_Element.clear();
            state.isP1Turn = !currentTurn; 
            GameState tempState = state; 
            validMoves = Trigger::getValidMoves(tempState);
            int choice = Printing::DisplayState_ShowValid_Ask(state, validMoves);
            state = applyMove(state, choice); 
            state.isP1Turn = currentTurn;
            state.SetReferences();
        }
        if (state.Attacker_HP_Check()) {  
            state.Attacker->Inflicted_Element.clear();
            GameState tempState = state; 
            validMoves = Trigger::getValidMoves(tempState);
            int choice = Printing::DisplayState_ShowValid_Ask(state, validMoves);
            state = applyMove(state, choice); 
            state.isP1Turn = currentTurn;
            state.SetReferences();
        }
        if (state.P_HP(*state.A_P) == 0 || state.P_HP(*state.D_P) == 0) {
            state.GameOver(state);
            break;
        }
    }
}

void applySkill   (GameState &state, int move, Ability &SpecialAttack, int d_index) {

    state.SetReferences();
    GameState tempState = state;
    
    Ability Attack = Ability();
    int A_P_SwapCost = state.A_P->SwapCost;
    string ReactionType;

    int A_P_Active = state.A_P->Active;

    if      (move == 0) {
        Trigger::T_Summon_On_End_Effects(state);
        //Trigger::T_Char_On_End_Effects(state);
        Trigger::T_Player_On_End_Effects(state);
    }
    else if(move ==  1) { 
        state.A_P->Active = state.A_P->Left(); }
    else if(move ==  2) { 
        state.A_P->Active = state.A_P->Right();}
    else if(move ==  3) { 
            Attack = state.Attacker->a;  }
    else if(move ==  4) { 
            Attack = state.Attacker->e;  }
    else if(move ==  5) { 
            Attack = state.Attacker->q;  }
    else if(move ==  6) { 
            Attack = state.Attacker->a2; }
    else if(move ==  7) { 
            Attack = state.Attacker->e2; }
    else if(move ==  8) { 
        state.A_P->Active = state.A_P->Left(); } 
    else if(move ==  9) { 
        state.A_P->Active = state.A_P->Right();} 
    else if(move == -2) { //Summon Attack
        Attack = SpecialAttack; }
    else if(move == -3) { //Follow Up Attack
        Attack = SpecialAttack;
    }
    else if(move == -5) {
        Attack = Trigger::Find_Prepare_Skill(state);
    }
    else if(move == -6) { 
        Attack = SpecialAttack;
    }
    else if(move == -7) {
        Attack = SpecialAttack;
    }

    state.SetReferences();

    
    Trigger::T_All_Atk_Status_and_Effects(state, Attack, A_P_SwapCost, move);
    if(Attack.Offensive) { 
        if(!Attack.self) {    
            ELogic::ApplyElement(state, Attack.Dmg, Attack.Element, *state.D_P, d_index);
            ReactionType = ELogic::CheckReaction(state, *state.D_P, d_index);
            ELogic::ApplyReaction(state, *state.A_P, *state.D_P, Attack, ReactionType, d_index, true);
        }
        else if (Attack.self) {
            ELogic::ApplyElement(state, Attack.Dmg, Attack.Element, *state.A_P, d_index);
            ReactionType = ELogic::CheckReaction(state, *state.A_P, d_index);
            ELogic::ApplyReaction(state, *state.D_P, *state.A_P, Attack, ReactionType, d_index, true);
        }
    }
    Trigger::T_All_Def_Status_and_Effects(state, Attack, A_P_SwapCost, move, d_index);
    if(Attack.Offensive) { 
        if(!Attack.self) {    
            state.ApplyDmg(Attack.Dmg, true);
            ELogic::ApplyReaction(state, *state.A_P, *state.D_P, Attack, ReactionType, d_index, false);
        }
        else if (Attack.self) {
            state.ApplyDmg(Attack.Dmg, false);
            ELogic::ApplyReaction(state, *state.D_P, *state.A_P, Attack, ReactionType, d_index, false);
        }
    }
    
    if(Attack.Defensive) { 
        
        state.ApplyHealing(Attack.Healing, true);
        ELogic::CleanseElement(state, Attack.Healing, Attack.Cleanse, 0);
        string Cleansed = ELogic::CheckReaction(state, *state.A_P, 0);
        ELogic::ApplyReaction(state, *state.D_P, *state.A_P, Attack, Cleansed, 0, false);
    }
    if (move == 1 || move == 2) {
        state.A_P->Dice -= A_P_SwapCost;
    }
    if (move == 3 || move == 4 || move == 5 || move == 6 || move == 7) { 
        state.A_P->Dice -= Attack.Dice_Cost;   
        state.Attacker->gainEnergy(Attack);
    }
       
    if      (Attack.swapEffect == 1) {  Player::SwappedLeft(*state.D_P, *tempState.D_P); state.D_P->updateQueue(); }
    else if (Attack.swapEffect == 2) { Player::SwappedRight(*state.D_P, *tempState.D_P); state.D_P->updateQueue(); }
    
    state.SetReferences(); 
    ResolveDeath(state); 
    
    //dont create xl burst before triggering fua but create quills

    if (      Attack.summon.Name != "No Summon") { Create::C_Summon(state, Attack.summon, state.A_P->summon_Zone); };
    if (    Attack.C_Status.Name != "No Status" && !Attack.C_Status.Follow_Up) {   Create::C_CharStatus(state, Attack.C_Status); }
    if (    Attack.P_Status.Name != "No Status" && !Attack.P_Status.Follow_Up) { Create::C_PlayerStatus(state, Attack.P_Status); }
    if (Attack.PrepareSkill.Name != "No Status" &&  Attack.IsPrepare)      { Create::C_PrepareSkill(state, Attack.PrepareSkill); }

    //trigger fua
    if(move >= 1 && move <= 7) {
        int currentSize1 = state.Attacker->SM.Statuses.size();
        for(auto &status : state.Attacker->SM.Statuses) { status.Shot = false; }
        auto it1 = state.Attacker->SM.Statuses.begin();
        while (it1 != state.Attacker->SM.Statuses.end()) {
            Status &s = *it1;
            if (s.Name != "No Status" && !s.Shot) {
                s.Shot = true;
                Trigger::T_Char_Status_fua(state, s, move);
            }
            if (it1 >= state.A_P->SM.Statuses.end()) {
                break;  // Prevent iterator from going out of bounds
            }
            if (state.Attacker->SM.Statuses.size() < currentSize1) {
                it1 = state.Attacker->SM.Statuses.begin();  
                currentSize1 = state.Attacker->SM.Statuses.size();
            }
            else { 
                ++it1; 
            }
        }

        int currentSize2 = state.Attacker->SM.Statuses.size();
        for(auto &status : state.A_P->SM.Statuses) { status.Shot = false; }
        auto it2 = state.A_P->SM.Statuses.begin();
        while (it2 != state.A_P->SM.Statuses.end()) {
            Status &s = *it2;
            if (s.Name != "No Status" && !s.Shot) {
                s.Shot = true;
                Trigger::T_Player_Status_fua(state, s, move);
            }
            if (it2 >= state.A_P->SM.Statuses.end()) {
                break; 
            }
            if (state.A_P->SM.Statuses.size() < currentSize2) {
                it2 = state.A_P->SM.Statuses.begin();
                currentSize2 = state.Attacker->SM.Statuses.size();  
            } else {
                ++it2;  // Move to next element
            }
        }
    }
    //create follow up statuses after follow up statuses trigger (think xiangling burst)
    if (Attack.C_Status.Name != "No Status" && Attack.C_Status.Follow_Up) {   Create::C_CharStatus(state, Attack.C_Status); }
    if (Attack.P_Status.Name != "No Status" && Attack.P_Status.Follow_Up) { Create::C_PlayerStatus(state, Attack.P_Status); }
    
    state.A_P->updateQueue();
    state.D_P->updateQueue();

    state.SetReferences();
}
void EndTurnEffects  (GameState &state) {
    for(int i = 0; i < 2; i++) { //status->summon update for both
        state.isP1Turn = !state.isP1Turn;
        state.SetReferences();
        SummonZone &summonZone = state.A_P->summon_Zone;

        // charlotte e??
        Trigger::T_Player_EndTurn_Status_Effects(state);

        int size = summonZone.Summons.size();
        for(auto &s : summonZone.Summons) { s.Shot = false; }
        auto it = summonZone.Summons.begin();
        while (it != summonZone.Summons.end()) {
            Summon &s = *it;
            if (s.Name != "No Summon" && !s.Shot) { 
                s.Shot = true;
                Trigger::T_Summon(state, s, summonZone);
            }
            if (it >= summonZone.Summons.end()) {
                break;
            }
            if (summonZone.Summons.size() < size) {
                it = summonZone.Summons.begin();
                size = summonZone.Summons.size();
            }
            else {
                ++it;
            }
        }
    }
    state.isP1Turn = !state.isP1Turn;

    state.A_P->summon_Zone.OPR_Update();
    state.D_P->summon_Zone.OPR_Update();

    for(int i = 0; i < 3; i++) { //unfreeze both
        state.P1.Char[i].Frozen = false;
        state.P2.Char[i].Frozen = false;
    }
    state.P1.End = false; state.P1.Dice = 8; 
    state.P2.End = false; state.P2.Dice = 8;
    state.Turn += 1; state.Action = 1;
}
GameState applyMove (const GameState &state, int move){ 

    GameState newState = state;

    newState.SetReferences();
    if (move >= 0 && move <= 7) { //trigger preAttack if move is 0 to 7 (not death)
        for(auto &p_status : newState.A_P->SM.Statuses) { 
            Trigger::T_Player_Status_PreAtk(newState, p_status, move);
        }
    }
    // now is the real move like in game
    // all status application is in applySkill since fua also go through applySkill
    bool FastSwap = false;
    if(move == 1 || move == 2) { // if swap check fast
        FastSwap = newState.A_P->CheckFastSwap(); 
    }
    Ability dummyAtk = Ability({0,0,0},{0,0,0},false,{0,0,0},0,false);
    applySkill(newState, move, dummyAtk,0);
    
    if(move == 0) { //end turn
        newState.A_P->End = true;
        //if(newState.isP1Turn && !newState.D_P->End) {
            //newState.P1_EndFirst = true;
        //}
        if(newState.A_P->End && newState.D_P->End) {
            EndTurnEffects(newState);
            return newState;
        }
    }

    if(!FastSwap) { //pass the turn if not fast swap + other side has not ended + not a death swap
        if(!newState.D_P->End && move != 8 && move != 9) { newState.isP1Turn = !newState.isP1Turn; }
        if(move <=7 && move >= 0) { newState.Action += 1; }
        if(move == -5)            { newState.Action += 1; }
    }
    newState.SetReferences();
    return newState;
};

int main() {
    
    Player P2 = {"P2", Xingqiu, Bennett,   Klee, 3}; 
    Player P1 = {"P1", Fischl, Collei, Chongyun, 1};  
    GameState state = {P1, P2, true};  
    
    while (state.P_HP(state.P1) > 0 && state.P_HP(state.P2) > 0) {
        vector<int> validMoves = Trigger::getValidMoves(state);     
        int choice = Printing::DisplayState_ShowValid_Ask(state, validMoves);
        //allow the Engine to play with itself
        //choice = minimax(state, 1, validMoves, state.isP1Turn); 
        cout << choice << endl;
        state = applyMove(state, choice);
    }    
}