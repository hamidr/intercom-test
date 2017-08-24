# intercom-test


This will parse a file in `data/customer.json` (unless given something different by the argument), and select those who are located within 100km radius of Dublin using their latitude/longitude, sort by their "user_id" field and print them.


1. Flatten is located in `src/intercom/utils.clj` and specified as `my-flatten` because Clojure already has it.

  1.1. Its tests are located in `tests/intercom/utils_test.clj`

2. The rest is about solving problem of finding all employees located within 100km of Dublin

  2.1.The tests are in `tests/intercom/geo_test.clj` and `tests/intercom/core_test.clj`


## Usage

You must have `lein` installed (https://leiningen.org/) because this is a Clojure Application.

### Run the tests:
`lein test`


### Run the app on unix(bsd/mac/linux) systems:
`lein run`
or
`lein run data/customer.json`


### Run the app on Windows systems:
`lein run data\customer.json`
