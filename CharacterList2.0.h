#ifndef CHARACTER_LIST_H
#define CHARACTER_LIST_H

#include "BasicStructs2.0.h"

Character Albedo    = {
    "Albedo", 10, 10, 0, "Sword",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{0,0,0},{0,0,0},false,{0,0,0},0,false, 3,0,1, Summon("Solar Isotoma",3,3,{1,0,0},{6,0,0},true,{0,0,0},0,false,true,1, true), 0, Status(), Status()}, 
    {{4,0,0},{6,0,0},true, {0,0,0},0,false, 3,2,0, Summon(), 0, Status(), Status()}};

Character Alhaitham = {
    "Alhaitham", 10, 10, 0, "Sword",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{2,0,0},{7,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status("Chisel-Light Mirror",false,2,3,false,true,false), Status()}, 
    {{4,0,0},{7,0,0},true, {0,0,0},0,false, 3,2,0, Summon(), 0, Status(), Status()}};
Character Amber     = {
    "Amber", 10, 10, 0, "Bow",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{0,0,0},{0,0,0},false,{0,0,0},0,false, 3,0,1, Summon("Baron Bunny", 1, 1, {2,0,0},{3,0,0},true,{0,0,0},0,false,true, 1), 0, Status(), Status()},
    {{2,2,2},{3,9,9},true, {0,0,0},0,false, 3,2,0, Summon(), 0, Status(), Status()}};
Character Ayato     = {
    "Ayato", 10, 10, 0, "Sword",
    {{2,0,0},{8,0,0},true,  {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{2,0,0},{2,0,0},true,  {0,0,0},0,false, 3,0,1, Summon(), 0, Status("Takimeguri Kenka",true,3,3), Status()}, 
    {{1,0,0},{2,0,0},true,  {0,0,0},0,false, 3,2,0, Summon("Garden of Purity",2,2,{2,0,0},{2,0,0},true,{0,0,0},0,false,true,0), 0, Status(), Status()}};
Character Barbara   = {
    "Barbara", 10, 10, 0, "Catalyst",
    {{1,0,0},{2,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{1,0,0},{2,0,0},true, {0,0,0},0,false, 3,0,1, Summon("Melody Loop",2,2,{0,0,0},{0,0,0},false,{1,1,1},2,true), 0, Status(), Status()},
    {{0,0,0},{0,0,0},false,{4,4,4},0, true, 3,3,0, Summon(), 0, Status(), Status()}};
Character Beidou    = {
    "Beidou", 10, 10, 0, "Claymore",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{0,0,0},{0,0,0},false,{0,0,0},0,false, 3,0,1, Summon(), 0, Status("Tidecaller Shield",true,2,2), Status(), true, Status("Wavestrider")},  
    {{2,0,0},{4,0,0},true, {0,0,0},0,false, 3,3,0, Summon(), 0, Status(), Status("Thunderbeast's Targe",false,2,2,false,true,false)}};

Character Bennett   = {
    "Bennett", 10, 10, 0, "Sword",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{3,0,0},{3,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()}, 
    {{2,0,0},{3,0,0},true, {2,0,0},0,false, 4,2,0, Summon(), 0, Status(), Status("Inspiration Field", false, 2, 2, false, false, true)}};
Character Candace   = {
    "Candace", 10, 10, 0, "Claymore",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{0,0,0},{0,0,0},false,{0,0,0},0,false, 3,0,1, Summon(), 0, Status("Heron Shield",true,2,2), Status(), true, Status("Heron Strike")},  
    {{2,0,0},{2,0,0},true, {0,0,0},0,false, 3,2,0, Summon(), 0, Status(), Status("Prayer of the Crimson Crown",false,2,2,true,false,false)}};

Character Charlotte = {
    "Charlotte", 10, 10, 0, "Catalyst",
    {{1,0,0},{1,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{1,0,0},{1,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status("Snappy Silhouette",true,2,2,true,true,false,true), Status()}, 
    {{1,0,0},{1,0,0},true, {1,1,1},0,true , 3,2,0, Summon("Newsflash Field",2,2,{1,0,0},{1,0,0},true,{1,0,0},0,true), 0, Status(), Status()}};

Character Chongyun  = {
    "Chong", 10, 10, 0, "Claymore",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{3,0,0},{1,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status("Chonghua Frost Field", false, 2, 2, false, false, false)}, 
    {{7,0,0},{1,0,0},true, {0,0,0},0,false, 3,3,0, Summon(), 0, Status(), Status()}};
Character Collei    = {
    "Collei", 10, 10, 0, "Bow",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0,                                                         Status(), Status()},  
    {{3,0,0},{7,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0,                                                         Status(), Status()},  
    {{2,0,0},{7,0,0},true, {0,0,0},0,false, 3,2,0, Summon("Cuilein-Anbar",2,2,{2,0,0},{7,0,0},true,{0,0,0},0,false), 0, Status(), Status()}};
Character Cyno      = {
    "Cyno", 10, 10, 0, "Polearm",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{3,0,0},{4,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{4,0,0},{4,0,0},true, {0,0,0},0,false, 4,2,0, Summon(), 0, Status(), Status()},
    Status("Pactsworn Pathclearer", 0, 999, false,false,false, true)};
Character Diona     = {
    "Diona", 10, 10, 0, "Bow",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{2,0,0},{1,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status("Cat-Claw Shield", true, 1, 1, false, false, false)}, 
    {{1,0,0},{1,0,0},true, {2,0,0},0,true,  4,2,0, Summon("Drunken Mist", 2, 2, {1,0,0},{1,0,0},true,{2,0,0},0,true), 0, Status(), Status()}};
Character Diluc     = { 
    "Diluc", 10, 10, 0, "Claymore",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{3,0,0},{3,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},
    {{8,0,0},{3,0,0},true, {0,0,0},0,false, 4,3,0, Summon(), 0, Status("Pyro Infusion", false, 2, 2,false,false,false), Status()},
    Status("DilucE+2", 0, 999, false, false, false, true)};
Character Dori      = { 
    "Dori", 10, 10, 0, "Claymore",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{2,0,0},{4,0,0},true, {0,0,0},0,false, 3,0,1, Summon("After-Sales Service Rounds", 1, 1,{1,0,0},{4,0,0},true,{0,0,0},0,false), 0, Status(), Status()},
    {{1,0,0},{4,0,0},true, {0,0,0},0,false, 3,2,0, Summon("Jinni",2,2,{0,0,0},{0,0,0},false,{2,0,0},0,true), 0, Status(), Status()},
    Status("DilucE+2", 0, 999, false, false, false, true)};

Character Eula      = { // summon loses dmg from sendoff
    "Eula", 10, 10, 0, "Claymore",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{2,0,0},{1,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status("Grimheart", true, 1, 2, false, false, true), Status()}, 
    {{2,0,0},{1,0,0},true, {0,0,0},0,false, 3,2,0, Summon("Lightfall Sword", 1, 1, {3,0,0},{8,0,0},true,{0,0,0},0,false), 0, Status(), Status()}};
Character E_Cube    = {
    "E_Cube", 8, 8, 0, "Other Weapons",
    {{1,0,0},{4,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {Ability()},  
    {{2,0,0},{4,0,0},true, {0,0,0},0,false, 3,2,0, Summon("Chains Of Warding Thunder",2,2,{1,0,0},{4,0,0},true,{0,0,0},0,false), 0, Status(), Status()},
    {Ability()}, 
    {{2,0,0},{4,0,0},true, {0,0,0},0,false, 5,0,1, Summon(), 0, Status(), Status(), true, Status("RPS Scissors")},
    {Status("Electro Crystal Core",1,1,false,false,false,true)}};

//Character E_Lector

Character Fischl    = {
    "Fischl", 10, 10, 0, "Bow",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(),                                              0, Status(), Status()},  
    {{1,0,0},{4,0,0},true, {0,0,0},0,false, 3,0,1, Summon("Oz",2,2,{1,0,0},{4,0,0},true,{0,0,0},0,false), 0, Status(), Status()},  
    {{4,2,2},{4,9,9},true, {0,0,0},0,false, 3,3,0, Summon(),                                              0, Status(), Status()}};
Character Ganyu     = {
    "Ganyu", 10, 10, 0, "Bow",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{1,0,0},{1,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status("Ice Lotus", true, 2, 2, false, false, false)},  
    {{2,1,1},{1,9,9},true, {0,0,0},0,false, 3,3,0, Summon("Sacred Cryo Pearl",2,2,{1,1,1},{1,9,9},true,{0,0,0},0,false), 0, Status(), Status()},
    {{2,2,2},{1,9,9},true, {0,0,0},0,false, 5,0,1, Summon(), 0, Status(), Status()},
    {Ability()}};
Character H_Herald  = {
        "H_Herald", 6, 6, 0, "Other Weapons",
        {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
        {{1,0,0},{2,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status(), true, Status("Rippling Blades")},  
        {{3,0,0},{2,0,0},true, {0,0,0},0,false, 3,2,0, Summon(), 0, Status(), Status("Curse of the Undercurrent",true,2,2,false,false,false,true)},
        {Ability()}, 
        {Ability()},
        {Status("Watery Rebirth",1,1,false,false,false,true)}};
    
Character Kaeya     = {
    "Kaeya", 10, 10, 0, "Sword",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{3,0,0},{1,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()}, 
    {{1,0,0},{1,0,0},true, {0,0,0},0,false, 4,2,0, Summon(), 0, Status(), Status("Icicle", true, 3, 3, false, true, false)}};

Character Klee      = {
    "Klee", 10, 10, 0, "Catalyst",
    {{1,0,0},{3,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{3,0,0},{3,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status("Explosive Spark",true, 1, 1,false,false,false), Status()},
    {{3,0,0},{3,0,0},true, {0,0,0},0,false, 3,3,0, Summon(), 0, Status(), Status("Sparks 'n' Splash", true,2,2,false,true,false,true)}};

Character Jadeplume = {
    "Jadeplume", 10, 10, 0, "Other Weapons",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{3,0,0},{7,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()}, 
    {{4,0,0},{7,0,0},true, {0,0,0},0,false, 3,2,0, Summon(), 0, Status(), Status("Icicle", true, 3, 3, false, true, false)},
    Status("Radical Vitality", 0, 3, false, false, false, true)};
Character Jean      = {
    "Jean", 10, 10, 0, "Sword",
    {{2,0,0},{8,0,0},true,  {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{3,0,0},{5,0,0},true,  {0,0,0},0,false, 3,0,1, Summon(), 2, Status(), Status()}, 
    {{0,0,0},{0,0,0},false, {2,2,2},0,true,  4,2,0, Summon("Dandelion Breeze",3,3,{1,0,0},{5,0,0},true,{1,0,0},0,true), 0, Status(), Status()}};
Character Keqing    = { // stilleto
    "Keqing", 10, 10, 0, "Sword",
    {{2,0,0},{8,0,0},true,  {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{3,0,0},{4,0,0},true,  {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()}, 
    {{4,3,3},{4,9,9},true,  {0,0,0},0,false, 4,3,0, Summon(), 0, Status(), Status()},
    Status("Lightning Stilleto", 0, 1, false, false, false, true)};
Character Mona      = {
    "Mona", 10, 10, 0, "Catalyst",
    {{1,0,0},{2,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{1,0,0},{2,0,0},true, {0,0,0},0,false, 3,0,1, Summon("Reflection",1,1,{1,0,0},{2,0,0},true,{0,0,0},0,false, true, 1), 0, Status(), Status()},
    {{4,0,0},{2,0,0},true, {0,0,0},0,false, 3,3,0, Summon(), 0, Status(), Status("Illusory Bubble",true,1,1,false,false,false)},
    {Ability()},
    {Ability()},
    {Status("Illusory Torrent",1,1,true,false,false,true)}};
Character P_Lector  = {
    "P_Lector", 6, 6, 0, "Other Weapons",
    {{1,0,0},{3,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{3,0,0},{3,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{3,0,0},{3,0,0},true, {0,0,0},0,false, 4,2,0, Summon("Darkfire Furnace",2,2,{1,1,1},{3,9,9},true,{0,0,0},0,false), 0, Status(), Status()},
    {Ability()}, 
    {Ability()},
    {Status("Fiery Rebirth",1,1,false,false,false,true)}};
Character Noelle    = {
    "Noelle", 10, 10, 0, "Claymore",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{1,0,0},{6,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status("Full Plate", true, 2, 2, false, false, false)}, 
    {{4,0,0},{6,0,0},true, {0,0,0},0,false, 4,2,0, Summon(), 0, Status("Sweeping Time", false, 2, 2, true, false, true), Status()}};
Character Razor     = {
    "Razor", 10, 10, 0, "Claymore",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{3,0,0},{4,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},
    {{3,0,0},{4,0,0},true, {0,0,0},0,false, 3,2,0, Summon(), 0, Status("The Wolf Within", false, 2, 2, false, true, false), Status()}};
Character Shenhe    = {
    "Shenhe", 10, 10, 0, "Polearm",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{2,0,0},{1,0,0},true ,{0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status("Icy Quill", true, 2, 2)}, 
    {{1,0,0},{1,0,0},true, {0,0,0},0,false, 3,2,0, Summon("Talisman Spirit", 2, 2, {1,0,0}, {1,0,0},true,{0,0,0},0,false,true,0), 0, Status(), Status()}}; 
Character Sucrose   = {
    "Sucrose", 10, 10, 0, "Catalyst",
    {{1,0,0},{5,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{3,0,0},{5,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 1, Status(), Status()}, 
    {{1,0,0},{5,0,0},true, {0,0,0},0,false, 3,2,0, Summon("Large Wind Spirit",3,3,{2,0,0},{5,0,0},true,{0,0,0},0,false), 0, Status(), Status()}};
Character Thoma     = {
    "Thoma", 10, 10, 0, "Polearm",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{2,0,0},{3,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status("Blazing Barrier", true,1,3)}, 
    {{2,0,0},{3,0,0},true, {0,0,0},0,false, 3,2,0, Summon(), 0, Status(), Status("Scorching Ooyoroi",true,2,2,false,true, true)}};
Character Tighnari  = {
    "Tighnari", 10, 10, 0, "Bow",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{2,0,0},{7,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status("Vijnana Suffusion", true, 2, 2), Status()}, 
    {{4,1,1},{7,9,9},true, {0,0,0},0,false, 3,2,0, Summon(), 0, Status(), Status()}};
Character Xiangling = {
    "XL", 10, 10, 0, "Polearm",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{0,0,0},{0,0,0},false,{0,0,0},0,false, 3,0,1, Summon("Guoba",2,2,{2,0,0},{3,0,0},true,{0,0,0},0,false), 0, Status(), Status()}, 
    {{3,0,0},{3,0,0},true, {0,0,0},0,false, 4,2,0, Summon(), 0, Status(), Status("Pyronado", true, 2, 2, false, true, false)}}; 
Character Xingqiu   = {
    "XQ", 10, 10, 0, "Sword",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{2,0,0},{2,0,0},true, {0,0,0},2,true,  3,0,1, Summon(), 0, Status(), Status("Rain Sword", true, 2, 2, false, false, false)}, 
    {{2,0,0},{2,0,0},true, {0,0,0},2,true,  3,2,0, Summon(), 0, Status(), Status("Rainbow Bladework", true, 3, 3, true, true, false)}}; 
Character Xinyan    = {
    "Xinyan", 10, 10, 0, "Claymore",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{2,0,0},{3,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status("Shield of Passion", true, 2, 2)}, 
    {{3,2,2},{8,9,9},true, {0,0,0},0,false, 3,2,0, Summon(), 0, Status(), Status("Festive Fires", true, 2, 2)}}; 
Character Yae_Miko  = {
    "Yae", 10, 10, 0, "Catalyst",
    {{1,0,0},{4,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{0,0,0},{0,0,0},false,{0,0,0},0,false, 3,0,1, Summon("Sesshou Sakura",3,6,{1,0,0},{4,0,0},true,{0,0,0},0,false, true, 1), 0, Status(), Status()}, 
    {{4,0,0},{4,0,0},true, {0,0,0},0,false, 3,2,0, Summon(), 0, Status(), Status("Tenko Check",true,1,1,false, false, true)}}; 
Character Yoimiya   = {
    "Yoimiya", 10, 10, 0, "Bow",
    {{2,0,0},{8,0,0},true, {0,0,0},0,false, 3,0,1, Summon(), 0, Status(), Status()},  
    {{0,0,0},{0,0,0},false,{0,0,0},0,false, 1,0,0, Summon(), 0, Status("Niwabi Enshou", true, 3, 3, false, false, false), Status()}, 
    {{3,0,0},{3,0,0},true, {0,0,0},0,false, 3,3,0, Summon(), 0, Status(), Status("Aurous Blaze", false, 2, 2, false, true, false)}}; 

#endif 