#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "texts/wordlist.txt"
#define TESTDICT "texts/test_worlist.txt"
#define TESTDICT_OVERFLOW "texts/test_overflow_worlist.txt"

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

START_TEST(test_dictionary_overflow)
    {
        hashmap_t hashtable[HASH_SIZE];
        ck_assert(load_dictionary(TESTDICT_OVERFLOW, hashtable));
        const char* words[3];
        int bucket=-1;
        words[0] ="Supercalifragilisticexpialidocious";
        words[1] ="super";
        words[2] ="accommodation";
        for (int i=0; i < 3; i++) {
            bucket=hash_function(words[i]); // We are re-using the hashfunction on purpose. See below
            ck_assert_str_eq(words[i],hashtable[bucket]->word);
        }
        // Here we are repeating the last test but our wordlist includes a superlong word.
        // We want to make sure it is ignored an no buffer is overflown
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

START_TEST(test_check_word_normal_punctuation_middle)
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
        FILE *fp = fopen("texts/test1.txt", "r");
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

START_TEST(test_check_words_punctuation_begining_ending)
    {
        hashmap_t hashtable[HASH_SIZE];
        load_dictionary(DICTIONARY, hashtable);
        char *misspelled[MAX_MISSPELLED];
        FILE *fp = fopen("texts/test3.txt", "r");
        int num_misspelled = check_words(fp, hashtable, misspelled);
        ck_assert_msg(num_misspelled == 0);
        // Here we are testing if the word has punctuation.
        // Punctuation in the beginning of the word should get striped out and word should be ok
        // Punctuation in the end of the word should get striped out and word should be ok
    }
END_TEST


START_TEST(test_check_words_spaces_numbers_special_chars)
    {
        hashmap_t hashtable[HASH_SIZE];
        load_dictionary(DICTIONARY, hashtable);
        char* expected[2];
        expected[0] = "2343gawre324";
        expected[1] = "2432super234";
        char *misspelled[MAX_MISSPELLED];
        FILE *fp = fopen("texts/test2.txt", "r");
        int num_misspelled = check_words(fp, hashtable, misspelled);
        ck_assert_msg(num_misspelled == 2);
        ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
        ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);

        // A word with all numbers should be considered correct
        // A bunch of spaces and tabs should not be a word and should be ignored
        // A word with all special characters or punctuation should be ignored
        // A word with numbers and letters should be incorrect

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

START_TEST(test_check_words_alice_in_wonderland)
    {
        hashmap_t hashtable[HASH_SIZE];
        load_dictionary(DICTIONARY, hashtable);
        char *misspelled[MAX_MISSPELLED];
        FILE *fp = fopen("texts/alice.txt", "r");
        int num_misspelled = check_words(fp, hashtable, misspelled);
        ck_assert_msg(num_misspelled == 1000);
        // Here we are testing if for a really long text and also over 1000 misspelled words
    }
END_TEST

START_TEST(test_check_words_onegin_english)
    {
        hashmap_t hashtable[HASH_SIZE];
        load_dictionary(DICTIONARY, hashtable);
        char *misspelled[MAX_MISSPELLED];
        FILE *fp = fopen("texts/onegin_j.txt", "r");
        int num_misspelled = check_words(fp, hashtable, misspelled);
        ck_assert_msg(num_misspelled == 1000);
        // Here we are testing if for some poetry and a really long misspelled list
    }
END_TEST

START_TEST(test_check_words_onegin_russian)
    {
        hashmap_t hashtable[HASH_SIZE];
        load_dictionary(DICTIONARY, hashtable);
        char *misspelled[MAX_MISSPELLED];
        FILE *fp = fopen("texts/onegin_r.txt", "r");
        int num_misspelled = check_words(fp, hashtable, misspelled);
        ck_assert_msg(num_misspelled == 1000);
        // Here we are testing if for same poetry in cyrillic characters
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
    tcase_add_test(check_word_case, test_dictionary_overflow);
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_word_normal_punctuation_middle);
    tcase_add_test(check_word_case, test_check_word_buffer_overflow);
    tcase_add_test(check_word_case, test_check_words_normal);
    tcase_add_test(check_word_case, test_check_words_punctuation_begining_ending);
    tcase_add_test(check_word_case, test_check_words_spaces_numbers_special_chars);
    tcase_add_test(check_word_case, test_check_words_alice_in_wonderland);
    tcase_add_test(check_word_case, test_check_words_onegin_english);
    tcase_add_test(check_word_case, test_check_words_onegin_russian);
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

