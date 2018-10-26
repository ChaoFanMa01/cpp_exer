#ifndef _GENETIC_HEADER_H
#define _GENETIC_HEADER_H

#include <iostream>
#include <utility>
#include <vector>
#include <initializer_list>
#include <cmath>
#include <random>
#include <ctime>
#include <set>

namespace genetic {
    typedef     std::vector<bool>::size_type   size_type;
    typedef     double                         fitness_type;
    typedef     int                            value_type;

    std::vector<bool> encode(value_type value, size_type len) {
       value_type          tmp = value >= 0 ? value : -value;
       std::vector<bool>   str;

       if (tmp == 0) {
           str.push_back(0);
           for (size_type i = 0; i < len; ++i)
               str.push_back(0);
           return str;
       }

       for (size_type i = 0; i < len; ++i) {
           if (tmp) {
               str.push_back(tmp % 2);
               tmp /= 2;
           } else {
               str.push_back(0);
           }
       }
       str.push_back(value >= 0 ? 0 : 1);

       for (std::vector<bool>::size_type head = 0, tail = str.size() -1; 
            head < tail; ++head, --tail) {
           tmp = str[head]; str[head] = str[tail]; str[tail] = tmp;
       }

       return str;
    }

    value_type decode(const std::vector<bool>& str) {
        value_type    val = 0;

        for (std::vector<bool>::size_type i = 1; i < str.size(); ++i)
            val += str[i] * std::pow(2, str.size() - i - 1);
        if (str[0])
            val = -val;
        return val;
    }

    struct chromosome {
        chromosome(value_type val, size_type len)
        : repr(encode(val, len)) {}

        chromosome(const chromosome& ch)
        : repr(ch.repr) {}

        chromosome(chromosome&& ch)
        : repr(std::move(ch.repr)) {}

        chromosome &operator=(const chromosome& ch) {
            repr = ch.repr;
            return *this;
        }

        chromosome &operator=(chromosome&& ch) {
            repr = std::move(ch.repr);
            return *this;
        }

        size_type size(void) const { return repr.size(); }

        bool operator[](const size_type& n) { return repr[n]; }
        const bool operator[](const size_type& n) const { return repr[n]; }

        // gene representation of this chromosome.
        std::vector<bool>    repr;
        fitness_type         fitness;
    };

    std::ostream& operator<<(std::ostream& os, const chromosome& ch) {
        os << "chromosome: " << decode(ch.repr) << ", string: ";
        for (size_type i = 0; i < ch.size(); ++i)
            os << ch[i];
        return os;
    }

    class population {
    public:
        typedef std::vector<chromosome>::iterator         iterator;
        typedef std::vector<chromosome>::const_iterator   const_iterator;

        population()
        : popu(std::vector<chromosome>()) {}

        template <class Itr>
        population(Itr b, Itr e, size_type pre)
        : popu(std::vector<chromosome>()) {
            for (Itr cnt = b; cnt != e; ++cnt)
                popu.push_back(chromosome(*cnt, pre));
        }

        population(const std::initializer_list<value_type>& il, size_type pre)
        : popu(std::vector<chromosome>()) {
            for (auto &e : il)
                popu.push_back(chromosome(e, pre));
        }

        population(const population& p)
        : popu(p.popu) {}

        population(population&& p)
        : popu(std::move(p.popu)) {}

        population &operator=(const population& p) {
            popu = p.popu;
            return *this;
        }

        population &operator=(population&& p) {
            popu = std::move(p.popu);
            return *this;
        }

        iterator begin(void) noexcept { return popu.begin(); }
        const_iterator begin(void) const noexcept { return popu.begin(); }

        iterator end(void) noexcept { return popu.end(); }
        const_iterator end(void) const noexcept { return popu.end(); }

        size_type size(void) const { return popu.size(); }

        chromosome &operator[](size_type n) { return popu[n]; }
        const chromosome &operator[](size_type n) const { return popu[n]; }

        chromosome &front(void) { return popu.front(); }
        const chromosome &front(void) const { return popu.front(); }
        
        chromosome &back(void) { return popu.back(); }
        const chromosome &back(void) const { return popu.back(); }

        void push_back(const chromosome& ch) { popu.push_back(ch); }
        void push_back(chromosome&& ch) { popu.push_back(ch); }

        void pop_back(void) { popu.pop_back(); }

        void clear(void) noexcept { popu.clear(); }

    private:
        std::vector<chromosome>    popu;
    };

    std::ostream& operator<<(std::ostream& os, const population& pp) {
        for (auto &ch : pp)
            os << ch << std::endl;
        return os;
    }

    class GA {
    public:
        // the population size.
        static const size_type     PN = 10;
        static const fitness_type  Pc;
        static const fitness_type  Pm;

        GA(const size_type& p,
           const value_type& l,
           const value_type& u,
           const size_type& g,
           fitness_type (*func)(value_type))
           : pp(population()), 
             precision(p),
             ui(std::uniform_int_distribution<int>(l, u)),
             ud(std::uniform_real_distribution<double>(0.0, 1.0)),
             uu(std::uniform_int_distribution<unsigned>(0, PN - 1)),
             up(std::uniform_int_distribution<unsigned>(0, p - 1)),
             e(std::default_random_engine(std::time(0))),
             lower(l), upper(u),
             generation(g),
             max(0),
             eval_func(func) {}
        
        /* @fn prepare()
         * Generate the initial population.
         */
        void prepare(void) {
            value_type    tmp;
            for (size_type i = 0; i < PN; ++i)
                pp.push_back(chromosome(ui(e), precision));
        }

        /* @fn total_fitness()
         * Calculate the total fitness of population.
         */
        fitness_type total_fitness(void) const {
            fitness_type    total = 0;
            for (auto &ch : pp)
                total += eval_func(decode(ch.repr));
            return total;
        }

        /* @fn compute_fitness()
         * Compute fitness for each chromosome in current population.
         */
        void compute_fitness(void) {
            fitness_type total = total_fitness();
            for (auto &ch : pp)
                ch.fitness = eval_func(decode(ch.repr)) / total;
        }
        
        /* @fn selection()
         * select the intermediate generation of a iteration,
         * using the remainder stochastic sampling method.
         */
        void selection(void) {
            population     intermediate;

            for (size_type i = 0; i < PN; ++i)
                intermediate.push_back(pp[roulette_wheel()]);
            pp = intermediate;
        }

        /* @fn roulette_wheel()
         * choose an chromosome using the roulette wheel method.
         */
        size_type roulette_wheel(void) {
            std::vector<fitness_type>    wheel;
            fitness_type                 cnt = 0;
            double                       p = ud(e);

            for (auto &ch : pp) {
                cnt += ch.fitness;
                wheel.push_back(cnt);
            }

            for (size_type i = 0; i < wheel.size(); ++i)
                if (p < wheel[i])
                    return i;
            std::cout << "wheel error" << std::endl;
            return 0;
        }

        /* @fn shuffle_pair()
         * shuffle the chromosomes in current population.
         */
        void shuffle(void) {
            std::set<size_type>     record;
            population              tmp;
            size_type               cnt, hld;

            while ((cnt = record.size()) < PN) {
                while (record.size() == cnt) {
                    hld = uu(e);
                    record.insert(hld);
                }
                tmp.push_back(pp[hld]);
            }
            pp.clear();
            pp = tmp;
        }

        /* @fn crossover()
         * crossover operation of two chromosomes.
         */
        void crossover(const size_type& i, const size_type& j) {
            size_type    pos = up(e);
            std::vector<bool>    tmp_i, tmp_j;

            tmp_i.push_back(0); tmp_j.push_back(0);
            for (size_type cnt = 0; cnt < precision; ++cnt)
                if (cnt < pos) {
                    tmp_i.push_back(pp[i].repr[cnt + 1]);
                    tmp_j.push_back(pp[j].repr[cnt + 1]);
                } else {
                    tmp_i.push_back(pp[j].repr[cnt + 1]);
                    tmp_j.push_back(pp[i].repr[cnt + 1]);
                }
            if (lower <= decode(tmp_i) && decode(tmp_i) <= upper &&
                lower <= decode(tmp_j) && decode(tmp_j)) {
                pp[i].repr.clear(); pp[i].repr = tmp_i;
                pp[j].repr.clear(); pp[j].repr = tmp_j;
            }
        }

        /* @fn recombination()
         * the recombination step.
         */
        void recombination(void) {
            for (size_type i = 0; i < PN / 2; ++i)
                if (ud(e) < Pc)
                    crossover(i * 2, i * 2 + 1);
        }

        void mutation(void) {
            for (auto &ch : pp)
                for (size_type i = 0; i < precision; ++i)
                    if (ud(e) < Pm) {
                        ch.repr[i + 1] = !ch.repr[i + 1];
                        if (decode(ch.repr) < lower || decode(ch.repr) > upper)
                            ch.repr[i + 1] = !ch.repr[i + 1];
                    }
        }

        void run(void) {
            update_max();
            print_population(std::cout);
            for (size_type i = 0; i < generation; ++i) {
                compute_fitness();
                selection();
                shuffle();
                recombination();
                mutation();
                update_max();
                print_population(std::cout);
                std::cout << "next generation: "<< std::endl;
            }
        }

        value_type get_max(void) const { return max; }

        void update_max(void) {
            fitness_type   tmp;
            for (auto &ch : pp) {
                tmp = eval_func(decode(ch.repr));
                if (tmp > eval_func(max))
                    max = decode(ch.repr);
            }
        }

        void print_population(std::ostream& os) const { os << pp; }
    private:
        population                                 pp;
        // encoding precision.
        size_type                                  precision;
        std::uniform_int_distribution<int>         ui;
        // random real number between 0 and 1, applied to roulette wheel.
        std::uniform_real_distribution<double>     ud;
        // random index to selection a chromosome in population.
        std::uniform_int_distribution<unsigned>    uu;
        // random index to select a gene in chromosome.
        std::uniform_int_distribution<unsigned>    up;
        std::default_random_engine                 e;
        // the lower bound of the possible value.
        value_type                                 lower;
        // the upper bound of the possible value.
        value_type                                 upper;
        // maximum iteration number.
        size_type                                  generation;
        // record the optimal value.
        value_type                                 max;
        // function to evaluate the fitness of a chromosome.
        fitness_type (*eval_func)(value_type);
    };

    const fitness_type GA::Pc = 0.6;
    const fitness_type GA::Pm = 0.005;
}

#endif
