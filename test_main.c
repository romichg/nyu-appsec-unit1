#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_worlist.txt"

START_TEST(test_dictionary_normal)
    {
        hashmap_t hashtable[HASH_SIZE];
        ck_assert(load_dictionary(TESTDICT, hashtable));
        const char* words[3];
        int bucket=-1;
        words[0] ="Supercalifragilisticexpialidocious";
        words[1] ="super";
        words[2] ="accommodation";
        for (int i=0; i < 3; i++) {
            bucket=hash_function(words[i]); // We are re-using the hashfunction on purpose. See below
            ck_assert_str_eq(words[i],hashtable[bucket]->word);
        }
        // Here we can test if certain words ended up in certain buckets
        // to ensure that our load_dictionary works as intended.
        // We are re-using the hashfunction on purpose.
        // 1. it is an external function that should be subject to its own tests. We are using the provided function
        // 2. The function can change and if it does it won't break our tests
    }
END_TEST

START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* incorrect_word = "Caoimhín";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(incorrect_word, hashtable));
    // Here we are checking for correct and incorrect word
}
END_TEST

START_TEST(test_check_word_normal_punctuation)
    {
        hashmap_t hashtable[HASH_SIZE];
        load_dictionary(DICTIONARY, hashtable);
        const char* correct_word = "Justice";
        const char* punctuation_word_2 = "pl.ace";
        ck_assert(check_word(correct_word, hashtable));
        ck_assert(!check_word(punctuation_word_2, hashtable));
        // Here we are testing if the word has punctuation.
        // Punctuation in the middle should be bad because it doesn't exist in dictionarry

        // Test here: What if a word begins and ends with "?
    }
END_TEST

START_TEST(test_check_words_normal)
    {
        hashmap_t hashtable[HASH_SIZE];
        load_dictionary(DICTIONARY, hashtable);
        char* expected[3];
        expected[0] = "sogn";
        expected[1] = "skyn";
        expected[2] = "betta";
        char *misspelled[MAX_MISSPELLED];
        FILE *fp = fopen("test1.txt", "r");
        int num_misspelled = check_words(fp, hashtable, misspelled);
        ck_assert(num_misspelled == 3);
        bool test = strlen(misspelled[0]) == strlen(expected[0]);
        int len1 = strlen(misspelled[0]);
        int len2 = strlen(expected[0]);
        ck_assert_msg(test, "%d!=%d", len1, len2);
        ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
        ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
        ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
    }
END_TEST


START_TEST(test_check_word_buffer_overflow)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char incorrect_word[500000];
    for(int i = 0; i < 499999; i++)
        incorrect_word[i] = 'A';
    incorrect_word[499999] = 0;
    ck_assert(!check_word(incorrect_word, hashtable));
}
END_TEST

Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_dictionary_normal);
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_word_normal_punctuation);
    tcase_add_test(check_word_case, test_check_word_buffer_overflow);
    tcase_add_test(check_word_case, test_check_words_normal);
    suite_add_tcase(suite, check_word_case);

    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

