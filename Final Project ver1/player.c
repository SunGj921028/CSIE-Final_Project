#include"phase.h"
#include"data.h"
#include"init.h"
#include"color.h"
#include"map.h"
#include"card.h"

extern sPlayer * p1;
extern sPlayer * p2;
extern sPlayer * p3;
extern sPlayer * p4;

//玩家行為
void player_move(){
    printf(RED"#This is Player 1's turn.\e[0m\n");
    //throw dice for player
    throw_dice(p1,0,1);
    //print_map

    while(1){
        //choose
        PASS;
        printf(BLUE"0 -> End this round!!\n");
        printf("1 -> Build\n");
        printf("2 -> Upgrade the Village to City!!\n");
        printf("3 -> Develop Card\n");
        printf("4 -> Trade\e[0m\n");
        //printf("Trade with Player\n");
        printf("Which move do you want to do ? (0-4): ");
        int32_t cho = 0;
        int32_t cho_move = 0;
        int32_t village_number = 0;
        int32_t road_number = 0;
        scanf("%d",&cho);
        switch(cho)
        {
            case 0:
                printf(RED"# Player 1's turn end.\e[0m\n");
                if(keep_index!=0){
                    save_develop_card(1);
                    p1->U_develop = 0;
                }
                return;
            case 1:
                PASS;
                printf(BLUE"0 -> Cancel\n");
                printf("1 -> Build the Village!!\n");
                printf("2 -> Build the Road!!\e[0m\n");
                printf("Which move do you want to do ? (0-2): ");
                scanf("%d",&cho_move);
                if(cho_move==0){
                    printf(RED"Build Action End!!\e[0m\n");
                    break;
                }else if(cho_move==1){
                    while(1){
                        printf("Which point you want to build Village ? (0-53): ");
                        scanf("%d",&village_number);
                        if(build_village(1,village_number,0,0) == -1){
                            continue;
                        }else{
                            p1->wheat--;
                            p1->brick--;
                            p1->wood--;
                            p1->sheep--;
                            p1->hand -= 4;
                            break;
                        }
                    }
                }else if(cho_move==2){
                    while(1){
                        printf("Which point you want to build Road ? (0-71): ");
                        scanf("%d",&road_number);
                        if(build_road(1,road_number,0) == -1){
                            continue;
                        }else{
                            p1->wood--;
                            p1->brick--;
                            p1->hand -= 2;
                            break;
                        }
                    }
                }else{
                    printf(RED"Wrong Input!!\e[0m\n");
                }
                //printf(RED"Build Action End!!\e[0m\n");
                break;
            case 2:
                //upgrade
                while(1){
                    map_print(1);
                    printf("Which Village you want to Upgrade ? (0-53): ");
                    scanf("%d",&village_number);
                    if(village_upgrade(1,village_number,0) != -1){
                        p1->wheat -= 2;
                        p1->iron -= 3;
                        p1->hand -= 5;
                        break;
                    }
                }
                break;
            case 3:
                PASS;
                printf(BLUE"0 -> Cancel\n");
                printf("1 -> Buy the develop card!!\n");
                printf("2 -> Use the develop card!!\e[0m\n");
                printf("Which move do you want to do ? (0-2): ");
                scanf("%d",&cho_move);
                if(cho_move==0){
                    printf(RED"Develop Card Action End!!\e[0m\n");
                    break;
                }else if(cho_move==1){
                    if(judge_buy_card(p1)){
                        get_develop_card(p1,1);
                        //printf("%d\n",keep_index);
                    }else{
                        printf("You don't have enough resource to buy!!\n");
                    }
                }else if(cho_move==2){
                    //use card
                    develop_card_state(p1,1,0);
                    int cho_card = 0;
                    printf("Which develop card you want to use ? (0-3): ");
                    scanf("%d",&cho_card);
                    if(cho_card<0 || cho_card>3){printf(RED"Wrong Input Of using card!!\e[0m\n"); break;}
                    if(use_card_state(1,cho_card,0)==-1){}
                }else{
                    printf(RED"Wrong Input!!\e[0m\n");
                }
                //printf(RED"Develop Card Action End!!\e[0m\n");
                break;
            case 4:
                PASS;
                printf(BLUE"0 -> Cancel\n");
                printf("1 -> Trade with Bank!!\n");
                printf("2 -> Trade with Harbor!!\e[0m\n");
                printf("Which move do you want to do ? (0-2): ");
                scanf("%d",&cho_move);
                int32_t trade_cho_give = 0;
                if(cho_move==0){
                    printf(RED"*Trade End*\e[0m\n");
                    break;
                }else if(cho_move==1){
                    //bank
                    list_can_trade(p1,1);
                    while(1){
                        //CLEAR;
                        //printf("-------------------->\n");
                        printf("Which 4 same resource do you want to give ? (0-4): ");
                        scanf("%d",&trade_cho_give);
                        if(trade_judge(p1,1,trade_cho_give)){
                            trade(p1,0,trade_cho_give,1);
                            break;
                        }else{
                            printf("-->You can't use this resource to trade with Bank!!\n");
                            printf("   You can press y to redo your choice,\n");
                            printf("   Or press n to end this action!\n");
                            PASS;
                            char cyn[2] = {0};
                            while(1){
                                printf("Do you want to keep trading with Bank ? (y or n): ");
                                scanf("%s",cyn);
                                if(strlen(cyn)!=1){ printf("Wrong Input!!\n"); continue;}
                                if(cyn[0]=='n'||cyn[0]=='N') {break;}
                                if(cyn[0]=='y'||cyn[0]=='Y') {break;}
                            }
                            if(cyn[0]=='n'||cyn[0]=='N') {break;}
                        }
                    }
                }else if(cho_move==2){
                    //harbor
                    //list the harbor you can trade and ask which or do you want
                    int32_t harbor_cho = 0;
                    int32_t port[6] = {0};
                    find_port(1,&port[0]);
                    bool ok = false;
                    for(int i=0;i<6;i++){
                        if(port[i]==1){ok = true; break;}
                    }
                    while(ok){
                        printf("1 -> Trade by 2:1\n");
                        printf("2 -> Trade by 3:1\n");
                        printf("Which harbor you want to trade with ? (1 or 2): ");
                        scanf("%d",&harbor_cho);
                        if(harbor_cho!=1 && harbor_cho!=2){ printf("Wrong Input!!\n"); continue;}
                        else{ break;}
                        //judge
                        bool can_trade = false;
                        if(harbor_cho==1){
                            for(int i=0;i<5;i++){
                                if(port[i]==1){ can_trade = true; break;}
                            }
                        }else if(harbor_cho==2){
                            if(port[5]==1){
                                break;
                            }
                        }
                        if(!can_trade){
                            if(harbor_cho==1){
                                printf("You don't have (2:1) harbor.\n");
                            }else if(harbor_cho==2){
                                printf("You don't have (3:1) harbor.\n");
                            }
                        }else{break;}
                    }
                    //cho_type 1->(2:1) 2->(3:1)
                    //list_can_trade(p1,harbor_cho);
                    if(ok){
                        int32_t res_cho = 0;
                        if(harbor_cho==1){
                            while(1){
                                printf(PURPLE"iron(0) "CYAN"wood(1) "YELLOW"wheat(2) "RED"brick(3) " L_GREEN"wool(4)\e[0m\n");
                                printf("Which same resource do you want to give for 2 to 1 trade? (0-4): ");
                                scanf("%d",&res_cho);
                                if(port[res_cho]!=1 || (trade_judge(p1,2,res_cho))==false){
                                    printf("You can't use this resource to trade!!\n");
                                    continue;
                                }else{
                                    break;
                                }
                            }
                        }else{
                            while(1){
                                printf(PURPLE"iron(0) "CYAN"wood(1) "YELLOW"wheat(2) "RED"brick(3) " L_GREEN"wool(4)\e[0m\n");
                                printf("Which same resource do you want to give for 2 to 1 trade? (0-4): ");
                                scanf("%d",&res_cho);
                                if((trade_judge(p1,3,res_cho))==false){
                                    printf("You can't use this resource to trade!!\n");
                                    continue;
                                }else{
                                    break;
                                }
                            }
                        }
                        trade(p1,0,res_cho,harbor_cho+1);
                    }
                }else{
                    printf(RED"Wrong Input!!\e[0m\n");
                }
                //printf(RED"*Trade End*\e[0m\n");
                break;
            default:
                printf(RED"Wrong Input!!\e[0m\n");
                break;
        }
        //CLEAR;
        //print_map()
    }
}