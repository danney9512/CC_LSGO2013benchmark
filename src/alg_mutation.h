#ifndef MUTATION__
#define MUTATION__

class Individual;

//-------------------------
//  Gaussian Mutation (GM)
//-------------------------

class GaussianMutation
{
public:
	explicit GaussianMutation(double pm = 0.05, double mu = 0.0, double sigma = 1.0) : pm_(pm), mu_(mu), sigma_(sigma) {}

	void SetMutationRate(double pm) { pm_ = pm; }
	double MutationRate() const { return pm_; }
	void SetMean(double mu) { mu_ = mu; }
	double Mean() const { return mu_; }
	void SetStandardDev(double sigma) { sigma_ = sigma; }
	double StandardDev() const { return sigma_; }

	bool operator()(Individual *c, double pm, double mu, double sigma) const;
	bool operator()(Individual *c, double pm) const { return operator()(c, pm, mu_, sigma_); }
	bool operator()(Individual *c) const { return operator()(c, pm_, mu_, sigma_); }

private:
	double pm_,     //mutation rate
		   mu_,     //mean
		   sigma_;  //standard deviation
};

//-------------------------
//  Polynomial Mutation (PM)
//-------------------------
class PolynomialMutation
{
public:
	explicit PolynomialMutation(double pm = 0.0, double eta = 20) : pm_(pm), eta_(eta) {}

	void SetMutationRate(double pm) { pm_ = pm; }
	double MutationRate() const { return pm_; }
	void SetDistributionIndex(double eta) { eta_ = eta; }
	double DistributionIndex() const { return eta_; }

	bool operator()(Individual *c, double pm, double eta) const;
	bool operator()(Individual *c) const
	{
		return operator()(c, pm_, eta_);
	}

private:
	double pm_, // mutation rate
		   eta_; // distribution index
};
#endif // !MUTATION__

