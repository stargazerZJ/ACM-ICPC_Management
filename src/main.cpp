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
#include <cstring>

int function_call_count = 0;

class ICPCManagementSystem {
public:

    ICPCManagementSystem() : contest_started_(false), frozen_(false), problems_(0),
                             team_count_(0), teams_(nullptr), rankings_array_(nullptr) {}

    ~ICPCManagementSystem();

    bool addTeam(const std::string &team_name);

    bool startContest(int duration, int problems);

    void
    submitSolution(const std::string &team_name, const std::string &problem_string, const std::string &result_string,
                   int time);

    void flush(bool log = true);

    bool freeze();

    bool scroll();

    int queryRanking(const std::string &team_name);

    bool
    querySubmission(const std::string &team_name, const std::string &problem_string, const std::string &result_string);

    void printRankings(bool debug = false);

    bool CommandHandler();

private:
    static const int kStatusCount = 4;
    static const int kMaxStringLength = 21;
    static const int kMaxProblemCount = 26;

    constexpr static const char *const kStatusString[kStatusCount + 1] = {"Accepted", "Wrong_Answer", "Runtime_Error",
                                                                          "Time_Limit_Exceed", "ALL"};

    struct Submission;

    struct Team;

    struct compareTeam {
        inline bool operator()(const Team *a, const Team *b) const;
    };

    std::set<std::string> names_list_;
    std::unordered_map<std::string, Team *> names_to_id_;
    std::set<Team *, compareTeam> rankings_;
    bool contest_started_;
    bool frozen_;
    int problems_;
    Team *teams_;
    int team_count_;
    Team **rankings_array_{};

    std::vector<Submission> submissions_;

    inline Team *getTeamPointer(const std::string &team_name) {
        auto it = names_to_id_.find(team_name);
        if (it == names_to_id_.end()) {
            return nullptr;
        } else {
            return it->second;
        }
    }

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

    int getProblemID(const std::string &problem_string) const {
        // if the problem_string is "ALL", return the number of problems
        return problem_string.size() > 1 ? problems_ : problem_string[0] - 'A';
    }

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

    inline bool exists() const {
        return team_ != nullptr;
    }
};

struct ICPCManagementSystem::Team {
    char name_[kMaxStringLength]{};
    int accepted_problems_;
    int frozen_problems_;
    int penalty_;
    int rank_;

    struct Problem {
        int unaccepted_submissions_;
        int accepted_time_;
        int submissions_after_frozen_;
        int unaccepted_submissions_after_frozen_;
        int accepted_time_after_frozen_;

        Problem() : unaccepted_submissions_(0), accepted_time_(0), submissions_after_frozen_(0),
                    unaccepted_submissions_after_frozen_(0), accepted_time_after_frozen_(0) {}

        inline int getPenalty() const {
            return unaccepted_submissions_ * 20 + accepted_time_;
        }

        inline void unfreeze() {
            unaccepted_submissions_ += unaccepted_submissions_after_frozen_;
            accepted_time_ = accepted_time_after_frozen_;
            submissions_after_frozen_ = 0;
            unaccepted_submissions_after_frozen_ = 0;
            accepted_time_after_frozen_ = 0;
        }

        inline bool accepted() const {
            return accepted_time_ != 0;
        }
    };

    Problem *problems_;
    Submission *last_submission_[kStatusCount + 1]{};
    int *accepted_time_{};

    Team() : accepted_problems_(0), frozen_problems_(0), penalty_(0), rank_(0), problems_(nullptr), accepted_time_(
            nullptr) {}

    void initialize(const char *name, int problems, int rank) {
        strcpy(name_, name);
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

    ~Team() {
        delete[] problems_;
        delete[] accepted_time_;
        for (auto &i: last_submission_) {
            delete[] i;
        }
    }

    inline bool isFrozen(int problem_id) const {
        return frozen_problems_ & (1 << problem_id);
    }

    inline int getFirstFrozenProblem() const {
        return __builtin_ctz(frozen_problems_);
    }

    inline int getAcceptedCount() const {
        return __builtin_popcount(accepted_problems_);
    }

    void getAcceptedTime() const {
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
        names_to_id_[name] = &teams_[i];
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
            team->getAcceptedTime();
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
                team->getAcceptedTime();
            }
            team->frozen_problems_ ^= 1 << problem_id;
            auto runner_up_after_unfreezing = rankings_.upper_bound(team);
            if (runner_up_before_unfreezing != runner_up_after_unfreezing) {
                char *replaced_team_name = (*runner_up_after_unfreezing)->name_;
                printf("%s %s %d %d\n", team->name_, replaced_team_name, team->getAcceptedCount(),
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
    printf("%s NOW AT RANKING %d\n", team->name_, team->rank_);
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
        printf("%s %c %s %d\n", team->name_, getProblemName(submission.problem_),
               kStatusString[submission.result_], submission.time_);
    }
    return true;
}

void ICPCManagementSystem::printRankings(bool debug) {
    for (int i = 0; i < team_count_; ++i) {
        Team *team = rankings_array_[i];
        printf("%s %d %d %d ", team->name_, team->rank_, team->getAcceptedCount(), team->penalty_);
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
//#ifndef ONLINE_JUDGE
//    freopen("../data/output.txt", "w", stdout);
//#endif
    ICPCManagementSystem ICPC_management_system;
    while (ICPC_management_system.CommandHandler());
#ifndef ONLINE_JUDGE
    // print to stderr
    fprintf(stderr, "function_call_count: %d\n", function_call_count);
#endif
    return 0;
}