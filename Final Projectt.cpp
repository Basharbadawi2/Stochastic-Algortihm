#include <iostream>
#include <vector>
#include <random>
#include<bits/stdc++.h>
using namespace std;
struct Reaction
{
    vector<int> reactants;
    vector<int> products;
    double rate;
};

class ChemicalSystem
{

private:
    vector<Reaction> reactions_;
    vector<int> concentrations_;
public:
    ChemicalSystem(const std::vector<Reaction>& reactions, const vector<int>& initial_concentrations)
        : reactions_(reactions), concentrations_(initial_concentrations) {}

    void simulate(double simulation_time)
    {
        default_random_engine generator;
        uniform_real_distribution<double> distribution(0.0, 1.0);

        double time = 0.0;
        while (time < simulation_time)
        {
            // Calculate propensity functions for each reaction
            vector<double> propensities;
            for (auto& reaction : reactions_)
            {
                double propensity = reaction.rate;
                for (auto& reactant : reaction.reactants)
                {
                    propensity *= concentrations_[reactant];
                }
                propensities.push_back(propensity);
            }

            // Calculate total propensity of the system
            double total_propensity = 0.0;
            for (auto& propensity : propensities)
            {
                total_propensity += propensity;
            }

            // Generate random numbers to determine which reaction occurs and when
            double r1 = distribution(generator);
            double r2 = distribution(generator);
            double time_until_next_reaction = -1.0 / total_propensity * log(r1);
            double cumulative_propensity = 0.0;
            int chosen_reaction = -1;
            for (int i = 0; i < reactions_.size(); i++)
            {
                cumulative_propensity += propensities[i];
                if (r2 * total_propensity <= cumulative_propensity)
                {
                    chosen_reaction = i;
                    break;
                }
            }

            // Update system by applying chosen reaction and advancing time
            for (auto& reactant : reactions_[chosen_reaction].reactants)
            {
                concentrations_[reactant]--;
            }
            for (auto& product : reactions_[chosen_reaction].products)
            {
                concentrations_[product]++;
            }
            time += time_until_next_reaction;

            // Print current state of the system
            cout << "Time: " << time << "\tConcentrations:";
            for (auto& concentration : concentrations_)
            {
                std::cout << " " << concentration;
            }
            cout << std::endl;
        }
    }
};

int main()
{
    vector<Reaction> reactions =
    {
        {{0}, {1}, 1.0}, // A -> B
        {{1}, {0}, 0.1}  // B -> A
    };
    vector<int> initial_concentrations = {100, 0};
    ChemicalSystem system(reactions, initial_concentrations);
    system.simulate(10.0);
    return 0;
}
