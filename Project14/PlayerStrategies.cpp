#pragma once
#include "PlayerStrategies.h"
using std::endl;

PlayerStrategy::PlayerStrategy()
{
}

PlayerStrategy::~PlayerStrategy()
{
}
HumanPlayerStrategy::HumanPlayerStrategy()
{
    P = nullptr;
}
HumanPlayerStrategy::~HumanPlayerStrategy()
{
}
void HumanPlayerStrategy::issueOrder()
{

    while (true)
    {
        cout << "you can issueOrder:(0 exit)" << std::endl;
        for (int i = 0; i < P->getOrders()->size(); i++)
        {
            cout << i + 1 << "." << P->getOrders()->at(i)->getordertype() << std::endl; // 显示现在的order
        }
        char c;
        int number;
        std::cin >> number; // 用户输入一个选择
        if (number == 0)
        {
            break;
        }
        else if (number > 0 && number < P->getOrders()->size())
        {
            if (dynamic_cast<DeployOrder *>(P->getOrders()->at(number - 1))) // 类型为DeployOrder
            {
                cout << "choice one:" << std::endl;
                for (size_t i = 0; i < P->defends.size(); i++)
                {
                    cout << i + 1 << "." << P->defends[i]->getName() << std::endl; // 显示用户的Territory
                }
                int t;
                std::cin >> t; // 输入一个显示用户的Territory
                if (t > 0 && t < P->defends.size())
                {
                    cout << "input number:" << std::endl;
                    int num;
                    std::cin >> num; // 输入数量
                    cout << "you want "
                         << " DeployOrder "
                         << P->defends[t - 1]->getName()
                         << " number "
                         << num
                         << " (Y/N)" << std::endl;
                    std::cin >> c;
                    if (c == 'Y')
                    {
                        P->issueOrder(num, P->defends[t - 1]);
                    }
                }
            }
            else if (dynamic_cast<AdvanceOrder *>(this)) // AdvanceOrder 不应该存在
            {
                cout << "error! this only take effect by attack and defend!" << std::endl;
            }
            else if (dynamic_cast<BombOrder *>(this)) // BombOrder
            {
                cout << "choice one:" << std::endl;
                for (size_t i = 0; i < P->defends.size(); i++)
                {
                    cout << i + 1 << "." << P->defends[i]->getName() << std::endl;
                }
                int t;
                std::cin >> t;
                if (t > 0 && t < P->defends.size())
                {
                    cout << "you want "
                         << " BombOrder "
                         << P->defends[t - 1]->getName()
                         << " (Y/N)" << std::endl;
                    std::cin >> c;
                    if (c == 'Y')
                    {
                        P->issueOrder(CardType::BOMB, P->defends[t - 1]);
                    }
                }
            }
            else if (dynamic_cast<BlockadeOrder *>(this)) // BlockadeOrder
            {
                cout << "choice one:" << std::endl;
                for (size_t i = 0; i < P->defends.size(); i++)
                {
                    cout << i + 1 << "." << P->defends[i]->getName() << std::endl;
                }
                int t;
                std::cin >> t;
                if (t > 0 && t < P->defends.size())
                {
                    cout << "you want "
                         << " BlockadeOrder "
                         << P->defends[t - 1]->getName()
                         << " (Y/N)" << std::endl;
                    std::cin >> c;
                    if (c == 'Y')
                    {
                        P->issueOrder(CardType::BLOCKADE, P->defends[t - 1]);
                    }
                }
            }
            else if (dynamic_cast<AirliftOrder *>(this)) // AirliftOrder
            {
                cout << "choice one:" << std::endl;
                for (size_t i = 0; i < P->defends.size(); i++)
                {
                    cout << i + 1 << "." << P->defends[i]->getName() << std::endl;
                }
                int t1;
                std::cin >> t1;
                if (t1 > 0 && t1 < P->defends.size())
                {
                    cout << "choice two:" << std::endl;
                    for (size_t i = 0; i < P->defends.size(); i++)
                    {
                        cout << i + 1 << "." << P->defends[i]->getName() << std::endl;
                    }
                    int t2;
                    std::cin >> t2;
                    if (t2 > 0 && t2 < P->defends.size())
                    {
                        cout << "input number:" << std::endl;
                        int num;
                        std::cin >> num;

                        cout << "you want " << P->defends[t1 - 1]->getName()
                             << " AirliftOrder to " << P->defends[t2 - 1]->getName()
                             << " army: " << num
                             << " (Y/N)" << std::endl;
                        std::cin >> c;
                        if (c == 'Y')
                        {
                            P->issueOrder(CardType::AIRLIFT, P->defends[t1 - 1], num, P->defends[t2 - 1]);
                        }
                    }
                }
            }
            else if (dynamic_cast<NegotiateOrder *>(this)) // NegotiateOrder
            {
                vector<Player *> allp = P->GE->getPlayers();

                cout << "choice one:" << std::endl;
                for (size_t i = 0; i < allp.size(); i++)
                {
                    cout << i + 1 << "." << allp[i]->getName() << std::endl;
                }
                int t1;
                std::cin >> t1;
                if (t1 > 0 && t1 < allp.size())
                {
                    char c;
                    cout << "you want "
                         << " NegotiateOrder  " << allp[t1 - 1]->getName()
                         << " (Y/N)" << std::endl;
                    std::cin >> c;
                    if (c == 'Y')
                    {
                        P->issueOrder(allp[t1 - 1]);
                    }
                }
            }
        }

        cout << "continue? Y/N" << std::endl;
        std::cin >> c;
        if (c == 'N')
        {
            break;
        }
    }
}
void HumanPlayerStrategy::toDefend()
{
    P->rebuildDefendList();
    while (true)
    {
        cout << "you can defend:" << std::endl;
        for (size_t i = 0; i < P->defends.size(); i++)
        {
            cout << i + 1 << "." << P->defends[i]->getName() << std::endl; // 显示所有的Territory
        }
        int fromadd;
        int toadd;
        int num;
        char c;
        cout << "select number from:" << std::endl;
        std::cin >> fromadd;
        if (fromadd > 0 && fromadd < P->defends.size())
        {
            vector<Territory *> lju = P->attacks[fromadd - 1]->getNeighbours();
            for (int i = 0; i < lju.size(); i++)
            {
                cout << i + 1 << "." << lju[i]->getName() << std::endl; // 显示已选Territory的邻居Territory
            }
            cout << "select move to" << std::endl;
            std::cin >> toadd;
            if (toadd > 0 && toadd < lju.size())
            {
                /* code */

                cout << "input number:" << std::endl;
                std::cin >> num;
                cout << "you want " << P->defends[fromadd - 1]->getName()
                     << " move to " << lju[toadd - 1]->getName()
                     << " army: " << num
                     << " (Y/N)" << std::endl;
                std::cin >> c;
                if (c == 'Y')
                {
                    P->issueOrder(P->defends[fromadd - 1], num, lju[toadd - 1]); // 提交
                }
            }
        }
        cout << "continue? Y/N" << std::endl;
        std::cin >> c;
        if (c == 'N')
        {
            break;
        }
    }
}
void HumanPlayerStrategy::toAttack()
{
    P->rebuildAttackList();
    while (true)
    {
        cout << "you can attacks" << std::endl;
        for (int i = 0; i < P->attacks.size(); i++)
        {
            cout << i + 1 << "." << P->attacks[i]->getName() << std::endl; // 显示所有可以攻击的Territory
        }
        int atted;
        int att;
        int num;
        char c;
        cout << "select you want attack" << std::endl;
        std::cin >> atted;
        if (atted > 0 && atted < P->attacks.size())
        {
            vector<Territory *> lju = P->attacks[atted]->getNeighbours();
            for (int i = 0; i < lju.size(); i++)
            {
                cout << i + 1 << "." << lju[i]->getName() << std::endl; // 我能派出的军队
            }
            cout << "select you attack" << std::endl;
            std::cin >> att;
            if (att > 0 && att < lju.size())
            {
                cout << "input number:" << std::endl;
                std::cin >> num;
                cout << "you want " << lju[att - 1]->getName()
                     << " attack to " << P->attacks[atted - 1]->getName()
                     << " army: " << num
                     << " (Y/N)" << std::endl;
                std::cin >> c;
                if (c == 'Y')
                {
                    P->issueOrder(P->attacks[atted - 1], num, lju[att - 1]);
                }
            }
        }
        cout << "continue? Y/N" << std::endl;
        std::cin >> c;
        if (c == 'N')
        {
            break;
        }
    }
}

AggressivePlayerStrategy::AggressivePlayerStrategy()
{
    P = nullptr;
}
AggressivePlayerStrategy::~AggressivePlayerStrategy()
{
}
void AggressivePlayerStrategy::issueOrder()
{
    int max = -1;
    Territory *maxt = 0;
    for (int i = 0; i < P->territores.size(); i++)
    {
        if (P->territores[i]->getArmyUnit() > max)
        {
            max = P->territores[i]->getArmyUnit();
            maxt = P->territores[i]; // 找到所有Territory中最强大的Territory
        }
    }
    if (max != -1)
    {
        P->issueOrder(P->getArmyUnit(), maxt); // 全部增加到maxt
    }
}
void AggressivePlayerStrategy::toDefend()
{
    for (int i = 0; i < P->territores.size(); i++)
    {
        int max = P->territores[i]->getArmyUnit();
        Territory *maxt = 0;
        vector<Territory *> ne = P->territores[i]->getNeighbours();
        for (int j = 0; j < ne.size(); j++)
        {
            if (ne[j]->getArmyUnit() > max)
            {
                max = ne[j]->getArmyUnit();
                maxt = ne[j]; // 找到每个Territory的邻居Territory中兵数最多的 进行兵力集中
            }
        }
        if (max != P->territores[i]->getArmyUnit())
        {
            P->issueOrder(P->territores[i], P->territores[i]->getArmyUnit(), maxt);
        }
    }
}
void AggressivePlayerStrategy::toAttack()
{
    vector<Player *> allp = P->GE->getPlayers();
    int max = -1;
    Player *maxp = 0;
    for (int i = 0; i < allp.size(); i++)
    {
        if (allp[i] != P)
        {
            if (max < allp[i]->getallarmyUnit())
            {
                max = allp[i]->getallarmyUnit();
                maxp = allp[i]; // 找到所有玩家中最强大的玩家
            }
        }
    }

    for (int i = 0; i < P->attacks.size(); i++)
    {
        if (P->attacks[i]->getPlayer() == maxp) // 选中强大玩家的Territory
        {
            for (int j = 0; j < P->attacks[i]->getNeighbours().size(); j++)
            {
                if (P->attacks[i]->getNeighbours()[j]->getPlayer() == P)
                {
                    P->issueOrder(P->attacks[i]->getNeighbours()[j], P->attacks[i]->getNeighbours()[j]->getArmyUnit(), P->attacks[i]);
                }
            }
        }
    }
}

BenevolentPlayerStrategy::BenevolentPlayerStrategy()
{
    P = nullptr;
}
BenevolentPlayerStrategy::~BenevolentPlayerStrategy()
{
}
void BenevolentPlayerStrategy::issueOrder()
{
    int min = INT32_MAX;
    Territory *mint = 0;
    for (int i = 0; i < P->territores.size(); i++)
    {
        if (P->territores[i]->getArmyUnit() < min)
        {
            min = P->territores[i]->getArmyUnit();
            mint = P->territores[i]; // 找到自己最弱小的Territory
        }
    }
    if (min != INT32_MAX)
    {
        P->issueOrder(P->getArmyUnit(), mint);
    }
}
void BenevolentPlayerStrategy::toDefend()
{
    for (int i = 0; i < P->defends.size(); i++)
    {
        vector<Territory *> lj = P->defends[i]->getNeighbours();
        for (int j = 0; j < lj.size(); j++)
        {
            if (lj[j]->getPlayer() == P && P->defends[i]->getArmyUnit() > lj[j]->getArmyUnit())
            {
                P->issueOrder(P->defends[i], (P->defends[i]->getArmyUnit() - lj[j]->getArmyUnit()) / 2, lj[j]); // 如果邻居Territory兵力比自己小就部署过去一部分
            }
        }
    }
}
void BenevolentPlayerStrategy::toAttack()
{
}

NeutralPlayerStrategy::NeutralPlayerStrategy()
{
    P = nullptr;
}
NeutralPlayerStrategy::~NeutralPlayerStrategy()
{
}
void NeutralPlayerStrategy::issueOrder()
{
}
void NeutralPlayerStrategy::toDefend()
{
}
void NeutralPlayerStrategy::toAttack()
{
}

CheaterPlayerStrategy::CheaterPlayerStrategy()
{
    P = nullptr;
}
CheaterPlayerStrategy::~CheaterPlayerStrategy()
{
}
void CheaterPlayerStrategy::issueOrder()
{
    if (P->attacks.size() > 0) // 直接将敌对的Territory变为自己的
    {
        P->attacks[0]->setPlayer(P);
    }
}
void CheaterPlayerStrategy::toDefend()
{
}
void CheaterPlayerStrategy::toAttack()
{
}
