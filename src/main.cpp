//
// Created by zj on 10/13/2023.
//

#include <iostream>
#include <cstdio>
#include <string>
#include <set>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <vector>

/**
 * @brief The class of ICPCManagementSystem
 * @details The class of ICPCManagementSystem, including the functions of adding teams, starting contest, submitting solutions, flushing scoreboard, freezing scoreboard, scrolling scoreboard, querying ranking, querying submission, printing rankings and handling commands
 *
 *
 */
class ICPCManagementSystem {
public:

    /**
     * @brief Construct a new ICPCManagementSystem object
     * @details Construct a new ICPCManagementSystem object, initialize the contest_started_ to false, the frozen_ to false, the problems_ to 0, the team_count_ to 0, the teams_ to nullptr and the rankings_array_ to nullptr
     *
     */
    ICPCManagementSystem() : contest_started_(false), frozen_(false), problems_(0),
                             team_count_(0), teams_(nullptr), rankings_array_(nullptr) {}

    /**
     * @brief Destroy the ICPCManagementSystem object
     * @details Destroy the ICPCManagementSystem object, delete the rankings_array_ and the teams_
     */
    ~ICPCManagementSystem();

    /**
     * @brief Add a team
     * @details Add a team, including adding the team name to the names_list_, adding the team name and the pointer to the team to the name_to_pointer_ and printing the information
     *
     * @param team_name the name of the team
     * @log "[Info]Add successfully." if no error occurs
     * @error If the contest has started, print "[Error]Add failed: competition has started." and return false
     * @error If the team name is duplicated, print "[Error]Add failed: duplicated team name." and return false
     * @return true if the team is added successfully, false if the team is not added successfully
     */
    bool addTeam(const std::string &team_name);

    /**
     * @brief Start the contest
     * @details Start the contest, including initializing the problems_, the team_count_, the teams_, the rankings_array_, setting the contest_started_ to true and printing the information
     *
     * @param duration the duration of the contest
     * @param problems the number of problems
     * @log "[Info]Competition starts." if no error occurs
     * @error If the contest has started, print "[Error]Start failed: competition has started." and return false
     * @return true if the contest is started successfully, false if the contest is already started
     */
    bool startContest(int duration, int problems);

    /**
     * @brief Submit a solution
     * @details Submit a solution, it will the submission data of the team. If the scoreboard has been frozen, it will update the frozen problem data of the team. Otherwise, it will push the submission into the submission list, waiting for flushing to update the problem data of the team
     *
     * @param team_name the name of the team
     * @param problem_string the string of problem, including A, B, C, ..., X(the last problem)
     * @param result_string the string of result, including Accepted, Wrong_Answer, Runtime_Error, Time_Limit_Exceed
     * @param time the submission time
     * @log Nothing
     * @error It's guaranteed that the parameters are valid, and that the competition has started, so there is no error handling
     * @return void
     */
    void
    submitSolution(const std::string &team_name, const std::string &problem_string, const std::string &result_string,
                   int time);

    /**
     * @brief Flush the scoreboard
     * @details Flush the scoreboard, including updating the problem data of the teams, updating the rankings and updating the rankings_array_.
     * If the scoreboard has been frozen, it will only proceed the submissions before the scoreboard is frozen.
     * Otherwise, it will proceed all the submissions.
     * @log "[Info]Flush scoreboard." if log is true
     * @error It's guaranteed that the competition has started, so there is no error handling
     * @param log whether to print the information
     */
    void flush(bool log = true);

    /**
     * @brief Freeze the scoreboard
     * @details Freeze the scoreboard, including setting the frozen_ to true and logging.
     * When the scoreboard is frozen, the problem data of the teams will be updated when scrolling.
     * A problem will be frozen if the problem has not been accepted before the scoreboard is frozen, and a team has submitted a solution to the problem after the scoreboard is frozen.
     * The scoreboard can be frozen many times.
     *
     * @log "[Info]Freeze scoreboard." if no error occurs
     * @error If the scoreboard has been frozen, print "[Error]Freeze failed: scoreboard has been frozen." and return false
     * @error It's guaranteed that the competition has started.
     * @return true if the scoreboard is frozen successfully, false if the scoreboard is already frozen
     */
    bool freeze();

    /**
     * @brief Scroll the scoreboard
     * @details Scroll the scoreboard, including updating the problem data of the teams, updating the rankings, updating the rankings_array_ and unfreeze the scoreboard.
     * When scrolling, it will first print the scoreboard before scrolling. Note that a flushing will be performed before scrolling to proceed the submissions before the scoreboard is frozen.
     * Then, it will proceed the submissions after the scoreboard is frozen. It will unfreeze the first frozen problem of the last team who has frozen problems, and update the problem data of the teams. If the rank of the team is changed, it will print the information.
     * The output format is "[team_name] [replaced_team_name] [accepted_count] [penalty]"
     * At last, it will print the scoreboard after scrolling.
     * @log "[Info]Scroll scoreboard." if no error occurs
     * @error If the scoreboard has not been frozen, print "[Error]Scroll failed: scoreboard has not been frozen." and return false
     * @return true if the scoreboard is scrolled successfully, false if the scoreboard is not frozen
     */
    bool scroll();

    /**
     * @brief Query the ranking of a team
     * @details Query the ranking of a team after last flushing, including printing the information
     *
     * @param team_name the name of the team
     * @log "[Info]Complete query ranking." if no error occurs
     * @warning If the scoreboard has been frozen, print "[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled."
     * @error If the team is not found, print "[Error]Query ranking failed: cannot find the team." and return -1
     * @error It's guaranteed that the competition has started.
     * @return the rank of the team, -1 if the team is not found
     */
    int queryRanking(const std::string &team_name);

    /**
     * @brief Query the submission of a team
     * @details Query the submission of a team, including printing the information. It will always print the latest result even if the scoreboard has been frozen, or the scoreboard has not been flushed in time.
     * If the problem is not found, it will print "Cannot find any submission."
     * Otherwise, it will print "[team_name] [problem_name] [result] [time]"
     *
     * @param team_name the name of the team
     * @param problem_string the string of problem, including A, B, C, ..., X(the last problem) and ALL
     * @param result_string the result, including Accepted, Wrong_Answer, Runtime_Error, Time_Limit_Exceed and ALL
     * @log "[Info]Complete query submission." if no error occurs
     * @error If the team is not found, print "[Error]Query submission failed: cannot find the team." and return false
     * @error It's guaranteed that the competition has started.
     * @return true if the submission is found, false otherwise
     */
    bool
    querySubmission(const std::string &team_name, const std::string &problem_string, const std::string &result_string);

    /**
     * @brief Print the rankings
     * @details Print the rankings
     * It will print the information of each team, including the name, the rank, the number of accepted problems, the penalty and the problem data.
     * When printing the problem data:
     * If the problem has been accepted, it will print "+[unaccepted_submissions_before_accepted]". Specifically, if unaccepted_submissions_before_accepted is 0, it will print "+".
     * If the problem has not been accepted, it will print "-[unaccepted_submissions_before_accepted]". Specifically, if unaccepted_submissions_before_accepted is 0, meaning that the problem has not been attempted, it will print ".".
     * If the problem has been frozen, it will print "-[unaccepted_submissions_before_accepted]/[submissions_after_frozen]".
     *
     * @log Nothing
     * @error It's guaranteed that the competition has started, so there is no error handling
     * @param debug whether to print the information
     */
    void printRankings(bool debug = false);

    /**
     * @brief Handle the commands
     * @details Handle the commands, including reading the command, calling the corresponding function and printing the information
     * Supported commands:
     * ADDTEAM [team_name]  // add a team
     * START DURATION [duration_time] PROBLEM [problem_count]  // start the contest
     * SUBMIT [problem_name] BY [team_name] WITH [submit_status] AT [time]  // submit a solution
     * FLUSH  // flush the scoreboard
     * FREEZE  // freeze the scoreboard
     * SCROLL  // scroll the scoreboard
     * QUERYRANKING [team_name]  // query the ranking of a team
     * QUERYSUBMISSION [team_name] [problem_name] [submit_status]  // query the submission of a team
     * PRINT  // print the rankings
     * END  // end the contest
     *
     * @return true if the command is "END", false otherwise
     */
    bool CommandHandler();

private:
    static const int kStatusCount = 4; // the number of status, including Accepted, Wrong_Answer, Runtime_Error, Time_Limit_Exceed, ALL. ALL is used in querySubmission
    static const int kMaxStringLength = 21; // the maximum length of team names and commands, including '\0'
    static const int kMaxProblemCount = 26; // the maximum number of problems

    constexpr static const char *const kStatusString[kStatusCount + 1] = {"Accepted", "Wrong_Answer", "Runtime_Error",
                                                                          "Time_Limit_Exceed",
                                                                          "ALL"}; // the string of status, including Accepted, Wrong_Answer, Runtime_Error, Time_Limit_Exceed, ALL. ALL is used in querySubmission

    /**
     * @brief The struct of submission
     * @details The struct of submission, including the pointer to the team, the problem id, the result id and the time
     *
     * @param team_ The pointer to the team
     * @param problem_ The problem id
     * @param result_ The result id
     * @param time_ The submission time
     *
     */
    struct Submission;

    /**
     * @brief The struct of team
     * @details The struct of team, including the name, the number of accepted problems, the number of frozen problems, the penalty, the rank, the problems, the last submissions and the accepted time
     *
     * @param name_ The name of the team
     * @param accepted_problems_ The bitmask of accepted problems, updated when flushing or scrolling
     * @param frozen_problems_ The bitmask of frozen problems, updated when scrolling
     * @param penalty_ The penalty, updated when flushing or scrolling
     * @param rank_ The rank, updated when flushing or scrolling
     * @param problems_ The array of problems
     * @param last_submission_ The array of last submissions, including the last submission of all status or problem, and the last submission of each status and problem
     * @param accepted_time_ The array of accepted time, in ascending order, updated when flushing or scrolling, used when comparing teams
     */
    struct Team;

    /**
     * @brief The functor of comparing teams
     * @details The functor of comparing teams
     * the rule of comparing teams:
     * 1. the number of accepted problems, the more the better
     * 2. the penalty, the less the better
     * 3. the accepted time of each problem, in ascending order, the less the better
     * 4. the name of the team, in alphabetical order, the less the better. Since the teams_ array stores the teams in the order of the names, the pointer to the team is used to compare the names
     * Since the names of the teams are unique, no more comparison is needed.
     *
     * @param a the pointer to the first team
     * @param b the pointer to the second team
     * @return true if the first team is better than the second team, false otherwise
     */
    struct compareTeam {
        inline bool operator()(const Team *a, const Team *b) const;
    };

    std::set<std::string> names_list_; // the set of team names
    std::unordered_map<std::string, Team *> name_to_pointer_; // the map from team name to team pointer
    std::set<Team *, compareTeam> rankings_; // the set of teams, sorted by the number of accepted problems, the penalty and the accepted time
    bool contest_started_; // whether the contest has started
    bool frozen_; // whether the scoreboard has been frozen. The scoreboard can be frozen many times.
    int problems_; // the number of problems
    Team *teams_; // the array of teams
    int team_count_; // the number of teams
    Team **rankings_array_{}; // the array of teams, sorted by the rank, updated when flushing or scrolling

    std::vector<Submission> submissions_; // the vector of submissions, waiting for flushing

    /**
     * @brief Get the pointer to the team
     * @param team_name the name of the team
     * @return The pointer to the team
     */
    inline Team *getTeamPointer(const std::string &team_name) {
        auto it = name_to_pointer_.find(team_name);
        if (it == name_to_pointer_.end()) {
            return nullptr;
        } else {
            return it->second;
        }
    }

    /**
     * @brief Get the result id
     * @param result_string the string of result, including Accepted, Wrong_Answer, Runtime_Error, Time_Limit_Exceed, ALL
     * @return The result id, 0 for Accepted, 1 for Wrong_Answer, 2 for Runtime_Error, 3 for Time_Limit_Exceed, 4 for ALL
     */
    static int getResultID(const std::string &result_string) {
        if (result_string[0] == 'A') {
            if (result_string[1] == 'c') {
                // Accepted
                return 0;
            } else {
                // ALL (used in querySubmission)
                return 4;
            }
        } else if (result_string[0] == 'W') {
            // Wrong_Answer
            return 1;
        } else if (result_string[0] == 'R') {
            // Runtime_Error
            return 2;
        } else if (result_string[0] == 'T') {
            // Time_Limit_Exceed
            return 3;
        }
        return 0;
    }

    /**
     * @brief Get the problem id
     * @param problem_string the string of problem, including A, B, C, ..., X(the last problem), and ALL. ALL is used in querySubmission
     * @return The problem id, 0 for A, 1 for B, 2 for C, ..., problems_ - 1 for X, problems_ for ALL
     */
    int getProblemID(const std::string &problem_string) const {
        // if the problem_string is "ALL", return the number of problems
        return problem_string.size() > 1 ? problems_ : problem_string[0] - 'A';
    }

    /**
     * @brief Get the problem name
     * @param problem_id the problem id
     * @return The problem name, A for 0, B for 1, C for 2, ..., X for problems_ - 1
     */
    static char getProblemName(int problem_id) {
        return 'A' + problem_id;
    }
};

struct ICPCManagementSystem::Submission {
    Team *team_;
    int problem_;
    int result_;
    int time_;

    Submission() : team_(nullptr), problem_(0), result_(0), time_(0) {}

    Submission(Team *team, int problem, int result, int time) : team_(team), problem_(problem), result_(result),
                                                                time_(time) {}

    /**
     * @brief Check whether the submission exists
     * @details Check whether the submission exists, by checking whether the team pointer is nullptr
     *
     * @return true if the submission exists, false otherwise
     */
    inline bool exists() const {
        return team_ != nullptr;
    }
};

struct ICPCManagementSystem::Team {
    std::string name_;
    int accepted_problems_;
    int frozen_problems_;
    int penalty_;
    int rank_;

    /**
     * @brief The struct of problem
     * @details The struct of problem, including the number of unaccepted submissions, the accepted time, the number of submissions after the scoreboard is frozen, the number of unaccepted submissions after the scoreboard is frozen and the accepted time after the scoreboard is frozen
     *
     * @param unaccepted_submissions_ The number of unaccepted submissions before the problem is accepted, updated when flushing or scrolling
     * @param accepted_time_ The time when the problem is accepted, updated when flushing or scrolling
     * @param submissions_after_frozen_ The number of submissions after the scoreboard is frozen, updated when submitting. It will be reset to 0 when the scoreboard is scrolled
     * @param unaccepted_submissions_after_frozen_ The number of unaccepted submissions after the scoreboard is frozen, and before the problem is accepted. It will be added to unaccepted_submissions_ and reset to 0 when the scoreboard is scrolled
     * @param accepted_time_after_frozen_ The accepted time after the scoreboard is frozen. It will be used to update accepted_time_ when the problem is accepted when the scoreboard is scrolled
     */
    struct Problem {
        int unaccepted_submissions_;
        int accepted_time_;
        int submissions_after_frozen_;
        int unaccepted_submissions_after_frozen_;
        int accepted_time_after_frozen_;

        Problem() : unaccepted_submissions_(0), accepted_time_(0), submissions_after_frozen_(0),
                    unaccepted_submissions_after_frozen_(0), accepted_time_after_frozen_(0) {}

        /**
         * @brief Get the penalty
         * @details Get the penalty. The penalty is the number of unaccepted submissions * 20 + the accepted time
         * @return
         */
        inline int getPenalty() const {
            return unaccepted_submissions_ * 20 + accepted_time_;
        }

        /**
         * @brief Unfreeze the problem
         * @details Unfreeze the problem, including updating the accepted time, the number of unaccepted submissions and resetting the number of submissions after the scoreboard is frozen, the number of unaccepted submissions after the scoreboard is frozen and the accepted time after the scoreboard is frozen
         */
        inline void unfreeze() {
            unaccepted_submissions_ += unaccepted_submissions_after_frozen_;
            accepted_time_ = accepted_time_after_frozen_;
            submissions_after_frozen_ = 0;
            unaccepted_submissions_after_frozen_ = 0;
            accepted_time_after_frozen_ = 0;
        }

        /**
         * @brief Check whether the problem has been accepted
         * @details Check whether the problem has been accepted, by checking whether the accepted time is 0
         * if the accepted time is 0, the problem has not been accepted
         * if the accepted time is not 0, the problem has been accepted
         *
         * @return true if the problem has been accepted, false otherwise
         */
        inline bool accepted() const {
            return accepted_time_ != 0;
        }
    };

    Problem *problems_;
    Submission *last_submission_[kStatusCount + 1]{};
    int *accepted_time_{};

    Team() : accepted_problems_(0), frozen_problems_(0), penalty_(0), rank_(0), problems_(nullptr), accepted_time_(
            nullptr) {}

    /**
     * @brief Initialize the team
     * @param name the name of the team
     * @param problems the number of problems
     * @param rank the initial rank of the team
     */
    void initialize(std::string name, int problems, int rank) {
        name_ = std::move(name);
        accepted_problems_ = 0;
        frozen_problems_ = 0;
        penalty_ = 0;
        rank_ = rank;
        problems_ = new Problem[problems];
        accepted_time_ = new int[problems];
        for (auto &i: last_submission_) {
            i = new Submission[problems + 1];
        }
    }

    /**
     * @brief Destroy the team
     * @details Destroy the team, delete the problems_, the accepted_time_ and the last_submission_
     */
    ~Team() {
        delete[] problems_;
        delete[] accepted_time_;
        for (auto &i: last_submission_) {
            delete[] i;
        }
    }

    /**
     * @brief Check whether the team has frozen problems
     * @details Check whether the team has frozen problems, by checking whether the frozen_problems_ is not 0
     * @param problem_id the problem id
     * @return true if the team has frozen problems, false otherwise
     */
    inline bool isFrozen(int problem_id) const {
        return frozen_problems_ & (1 << problem_id);
    }

    /**
     * @brief Get the first frozen problem
     * @details Get the first frozen problem, by using __builtin_ctz
     * @return The first frozen problem
     */
    inline int getFirstFrozenProblem() const {
        return __builtin_ctz(frozen_problems_);
    }

    /**
     * @brief Get the number of accepted problems
     * @details Get the number of accepted problems, by using __builtin_popcount
     * @return The number of accepted problems
     */
    inline int getAcceptedCount() const {
        return __builtin_popcount(accepted_problems_);
    }

    /**
     * @brief Set the accepted time
     * @details Set the accepted_time_ array, and sort it in descending order
     */
    void setAcceptTime() const {
        int mask = accepted_problems_, i = 0;
        while (mask) {
            int problem_id = __builtin_ctz(mask);
            accepted_time_[i] = problems_[problem_id].accepted_time_;
            mask ^= 1 << problem_id;
            ++i;
        }
        std::sort(accepted_time_, accepted_time_ + i, std::greater<>());
    }
};

ICPCManagementSystem::~ICPCManagementSystem() {
    delete[] rankings_array_;
    delete[] teams_;
}

inline bool ICPCManagementSystem::compareTeam::operator()(const ICPCManagementSystem::Team *a,
                                                          const ICPCManagementSystem::Team *b) const {
    if (a->getAcceptedCount() != b->getAcceptedCount()) {
        return a->getAcceptedCount() > b->getAcceptedCount();
    }
    if (a->penalty_ != b->penalty_) {
        return a->penalty_ < b->penalty_;
    }
    const int accepted_problem_count = a->getAcceptedCount();
    for (int i = 0; i < accepted_problem_count; ++i) {
        if (a->accepted_time_[i] != b->accepted_time_[i]) {
            return a->accepted_time_[i] < b->accepted_time_[i];
        }
    }
    return a < b;
}

bool ICPCManagementSystem::addTeam(const std::string &team_name) {
    if (contest_started_) {
        puts("[Error]Add failed: competition has started.");
        return false;
    }
    if (names_list_.find(team_name) != names_list_.end()) {
        puts("[Error]Add failed: duplicated team name.");
        return false;
    }
    names_list_.insert(team_name);
    puts("[Info]Add successfully.");
    return true;
}

bool ICPCManagementSystem::startContest(int duration, int problems) {
    if (contest_started_) {
        puts("[Error]Start failed: competition has started.");
        return false;
    }
    problems_ = problems;
    team_count_ = static_cast<int>(names_list_.size());
    teams_ = new Team[team_count_];
    rankings_array_ = new Team *[team_count_];
    int i = 0;
    for (const auto &name: names_list_) {
        teams_[i].initialize(name.c_str(), problems, i + 1);
        rankings_.insert(rankings_.end(), &teams_[i]);
        name_to_pointer_[name] = &teams_[i];
        i++;
    }
    contest_started_ = true;
    puts("[Info]Competition starts.");
    return true;
}

void ICPCManagementSystem::submitSolution(const std::string &team_name, const std::string &problem_string,
                                          const std::string &result_string,
                                          int time) {
    Team *team = getTeamPointer(team_name);
    int result = getResultID(result_string);
    int problem_id = getProblemID(problem_string);
    Submission submission(team, problem_id, result, time);
    if (!frozen_) {
        // push the submission into the submission list, waiting for flushing
        submissions_.push_back(submission);
    } else {
        // update the problem data of the team
        Team::Problem &problem = team->problems_[problem_id];
        ++problem.submissions_after_frozen_;
        // If the problem has been accepted, do nothing
        if (!team->last_submission_[0][problem_id].exists()) {
            team->frozen_problems_ |= 1 << problem_id;
            if (result == 0) {
                // Accepted
                problem.accepted_time_after_frozen_ = time;
            } else {
                // Unaccepted
                ++problem.unaccepted_submissions_after_frozen_;
            }
        }
    }
    // update the last submission data of the team
    team->last_submission_[result][problem_id] = submission;
    team->last_submission_[result][problems_] = submission;
    team->last_submission_[kStatusCount][problem_id] = submission;
    team->last_submission_[kStatusCount][problems_] = submission;
}

void ICPCManagementSystem::flush(bool log) {
    for (auto submission: submissions_) {
        Team *team = submission.team_;
        int problem_id = submission.problem_;
        int result = submission.result_;
        int time = submission.time_;
        Team::Problem &problem = team->problems_[problem_id];
        if (problem.accepted()) {
            // If the problem has been accepted before flushing, do nothing
            continue;
        }
        if (result == 0) {
            // Accepted
            rankings_.erase(team);
            team->accepted_problems_ |= 1 << problem_id;
            problem.accepted_time_ = time;
            team->penalty_ += problem.getPenalty();
            team->setAcceptTime();
            rankings_.insert(team);
        } else {
            // Unaccepted
            ++problem.unaccepted_submissions_;
        }
    }
    submissions_.clear();
    int rank = 1;
    for (const auto &team: rankings_) {
        team->rank_ = rank;
        rankings_array_[rank - 1] = team;
        ++rank;
    }
    if (log)
        puts("[Info]Flush scoreboard.");
}

bool ICPCManagementSystem::freeze() {
    if (frozen_) {
        puts("[Error]Freeze failed: scoreboard has been frozen.");
        return false;
    }
    frozen_ = true;
    puts("[Info]Freeze scoreboard.");
    return true;
}

bool ICPCManagementSystem::scroll() {
    if (!frozen_) {
        puts("[Error]Scroll failed: scoreboard has not been frozen.");
        return false;
    }
    puts("[Info]Scroll scoreboard.");
    flush(false);
    printRankings();
    std::priority_queue<Team *, std::vector<Team *>, compareTeam> teams_with_frozen_problems;
    for (int i = 0; i < team_count_; ++i) {
        Team *team = rankings_array_[i];
        if (team->frozen_problems_) {
            teams_with_frozen_problems.push(team);
        }
    }
    while (!teams_with_frozen_problems.empty()) {
        Team *team = teams_with_frozen_problems.top();
        teams_with_frozen_problems.pop();
        int problem_id = team->getFirstFrozenProblem();
        Team::Problem &problem = team->problems_[problem_id];
        if (problem.accepted_time_after_frozen_) {
            rankings_.erase(team);
            auto runner_up_before_unfreezing = rankings_.upper_bound(team);
            problem.unfreeze();
            if (problem.accepted()) {
                team->accepted_problems_ |= 1 << problem_id;
                team->penalty_ += problem.getPenalty();
                team->setAcceptTime();
            }
            team->frozen_problems_ ^= 1 << problem_id;
            auto runner_up_after_unfreezing = rankings_.upper_bound(team);
            if (runner_up_before_unfreezing != runner_up_after_unfreezing) {
                const std::string &replaced_team_name = (*runner_up_after_unfreezing)->name_;
                printf("%s %s %d %d\n", team->name_.c_str(), replaced_team_name.c_str(), team->getAcceptedCount(),
                       team->penalty_);
                rankings_.insert(runner_up_after_unfreezing, team);
            }
            rankings_.insert(runner_up_after_unfreezing, team);
        } else {
            problem.unfreeze();
            team->frozen_problems_ ^= 1 << problem_id;
        }
        if (team->frozen_problems_) {
            teams_with_frozen_problems.push(team);
        }
    }
    flush(false);
    printRankings();
    frozen_ = false;
    return true;
}

int ICPCManagementSystem::queryRanking(const std::string &team_name) {
    Team *team = getTeamPointer(team_name);
    if (team == nullptr) {
        puts("[Error]Query ranking failed: cannot find the team.");
        return -1;
    }
    puts("[Info]Complete query ranking.");
    if (frozen_) {
        puts("[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled.");
    }
    printf("%s NOW AT RANKING %d\n", team->name_.c_str(), team->rank_);
    return team->rank_;
}

bool ICPCManagementSystem::querySubmission(const std::string &team_name, const std::string &problem_string,
                                           const std::string &result_string) {
    Team *team = getTeamPointer(team_name);
    if (team == nullptr) {
        puts("[Error]Query submission failed: cannot find the team.");
        return false;
    }
    int problem_id = getProblemID(problem_string);
    int result = getResultID(result_string);
    Submission &submission = team->last_submission_[result][problem_id];
    puts("[Info]Complete query submission.");
    if (!submission.exists()) {
        puts("Cannot find any submission.");
    } else {
        printf("%s %c %s %d\n", team->name_.c_str(), getProblemName(submission.problem_),
               kStatusString[submission.result_], submission.time_);
    }
    return true;
}

void ICPCManagementSystem::printRankings(bool debug) {
    for (int i = 0; i < team_count_; ++i) {
        Team *team = rankings_array_[i];
        printf("%s %d %d %d ", team->name_.c_str(), team->rank_, team->getAcceptedCount(), team->penalty_);
        for (int problem_id = 0; problem_id < problems_; ++problem_id) {
            Team::Problem &problem = team->problems_[problem_id];
            if (team->isFrozen(problem_id)) {
                printf("%d/%d", -problem.unaccepted_submissions_, problem.submissions_after_frozen_);
            } else {
                if (problem.accepted()) {
                    putchar('+');
                    if (problem.unaccepted_submissions_) {
                        printf("%d", problem.unaccepted_submissions_);
                    }
                } else {
                    if (problem.unaccepted_submissions_) {
                        printf("%d", -problem.unaccepted_submissions_);
                    } else {
                        putchar('.');
                    }
                }
            }
            putchar(' ');
        }
        putchar('\n');
    }
}

bool ICPCManagementSystem::CommandHandler() {
    static char command[kMaxStringLength];
    static char team_name[kMaxStringLength];
    static char problem_string[kMaxStringLength];
    static char result_string[kMaxStringLength];
    static int time;
    scanf("%s", command);
    if (command[0] == 'A') {
        // ADDTEAM
        scanf("%s", team_name);
        addTeam(team_name);
    } else if (command[0] == 'S' && command[1] == 'T') {
        // START DURATION [duration_time] PROBLEM [problem_count]
        int duration, problems;
        scanf(" DURATION %d PROBLEM %d", &duration, &problems);
        startContest(duration, problems);
    } else if (command[0] == 'S' && command[1] == 'U') {
        // SUBMIT [problem_name] BY [team_name] WITH [submit_status] AT [time]
        scanf("%s BY %s WITH %s AT %d", problem_string, team_name, result_string, &time);
        submitSolution(team_name, problem_string, result_string, time);
    } else if (command[0] == 'F' && command[1] == 'L') {
        // FLUSH
        flush();
    } else if (command[0] == 'F' && command[1] == 'R') {
        // FREEZE
        freeze();
    } else if (command[0] == 'S' && command[1] == 'C') {
        // SCROLL
        scroll();
    } else if (command[0] == 'Q' && command[6] == 'R') {
        // QUERY_RANKING [team_name]
        scanf("%s", team_name);
        queryRanking(team_name);
    } else if (command[0] == 'Q' && command[6] == 'S') {
        // QUERY_SUBMISSION [team_name] WHERE PROBLEM=[problem_name] AND STATUS=[status]
        scanf("%s WHERE PROBLEM=%s AND STATUS=%s", team_name, problem_string, result_string);
        querySubmission(team_name, problem_string, result_string);
    } else if (command[0] == 'E') {
        // END
        puts("[Info]Competition ends.");
        return false;
    }
    return true;
}

int main() {
    ICPCManagementSystem ICPC_management_system;
    while (ICPC_management_system.CommandHandler());
    return 0;
}