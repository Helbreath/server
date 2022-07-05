
#include "core.h"

#if !defined(HELBREATHX)
bool core::CheckLogin(Client * client, std::string & account, std::string & pass)
{
    try
    {
        // retrieve server account
        pqxx::work txn{ *pq_login };
        pqxx::row r{ txn.exec_params1("SELECT * FROM accounts WHERE email=$1LIMIT 1", pq_login->quote(account)) };
        txn.commit();

        if (r["pass"].as<std::string>() == pass)
        {
            return true;
        }
        log->info(fmt::format("Wrong pass [{}]", account));
        return false;
    }
    catch (pqxx::unexpected_rows &)
    {
        log->info(fmt::format("Creating account [{}]", account));
        try
        {
            // no rows exist - create new account
            pqxx::work txn{ *pq_login };
            pqxx::result r{ txn.exec_params(
                "INSERT INTO accounts (email, pass) VALUES ($1, $2)",
                pq_login->quote(account),
                pq_login->quote(pass)
            ) };
            txn.commit();
            return true;
        }
        catch (std::exception & ex)
        {
            log->critical(fmt::format("Error creating account [{}] - {}", account, ex.what()));
            DeleteClient(client, false, true);
        }
    }
    catch (std::exception & ex)
    {
        log->critical(fmt::format("Error querying login attempt for account [{}] - {}", account, ex.what()));
        DeleteClient(client, false, true);
    }
    return false;
}
#endif
