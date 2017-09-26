#ifndef __STATS_HPP__
#include <vector>

const float NORMALIZER =  2147483647.0;   //  divide rand() by it and result is in the open interval (0,1)

// statistics class declarations
// Statistics class is abstract base class with pure virtual functions
// not to be invoked as a run time object
class Statistics
{
    public:
        Statistics() : n(0) { }
        Statistics(unsigned u) : n(u) { }
        void displayData() const;
        unsigned getSize() const;
        float getMean() const;
        float getVar() const;
        float getStd() const;
        float getMin() const;
        float getMax() const;
        float getMedian() const;
        float getMad() const;
        float retrieveItem(unsigned) const;
    protected:
        unsigned n;
        std::vector<float> x;
        void display() const;
        virtual unsigned size() const;
        float mean() const;
        float min() const;
        float max() const;
        float median() const;
        float median(const std::vector<float>&) const;
        float mad() const;
        float rank(unsigned) const;
        float rank(const std::vector<float>&, unsigned) const;
        virtual float var() const = 0;
        virtual float std() const = 0;
};

class Population : public virtual Statistics {
    public:
        Population() : Statistics() { }
        Population(unsigned u) : Statistics(u) { load(); }
        // copy constructor
        Population(const Population& p) : Statistics(p.getSize()) { copy(p); }
    protected:
        void load();
        void copy(const Population& p);
        float var() const;
        float std() const;
};

class Sample : public Population {
    public:
        Sample() : Population() { }
        Sample(const Population& p) : Population(p) {}
        Sample(unsigned u, const Population& p) : Population(u) { }
    protected:
        float var() const;
        float std() const;
};

class RandomSample : public Sample {
    public:
        RandomSample(unsigned u, const Population& p) : Statistics(u) { load(u, p); }
    protected:
        void load(unsigned, const Population&);
};

class FrequencyAnalyzer : public Sample {
    public:
        FrequencyAnalyzer() : Statistics() { }
        FrequencyAnalyzer(unsigned u) : Statistics(u) { load(u); }
        void displayTable() const { display_table(); }
    protected:
        std::vector<int> data;  // keeping it integral - frequency count array
        void load(unsigned);
        void count_frequencies();
        virtual void display_table() const;
};

class TextAnalyzer : public FrequencyAnalyzer {
    public:
        TextAnalyzer(const char *);
        void printReport() const;
        float getCov() const;
        float getIoc() const;
        float getCorr() const;
    private:
        static const int C = 26;
        static const float english_data[C];
        int charcount;
        void load(const char* textfile);
        void display_table() const;
        unsigned size() const { return charcount; }
        float coefficient_of_variation() const;
        float index_of_coincidence() const;
        float english_correlation() const;
        void select_idx(const std::vector<float>&, int[]) const;
};
#endif
