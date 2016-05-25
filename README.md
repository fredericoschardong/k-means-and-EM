A simple implementation of k-means and EM algorithms running on a static dataset.

You need to have gnuplot installed to check the output.

To compile simply run:
`gcc -o main main.c -Wextra -Wall -lm`

And then:
`./main`

Where the first parameter is the IMDB folder, second is the starting index and third is the last document index.

Here is what the output looks like:
```
Reading from: IMDB
Start index: 12
End index: 24011
Total examples to learn: 21600
Total examples to test: 2400

Mean True Positive: 1838.2, Standard Deviation: 181.54
Mean True Negative: 2053.9, Standard Deviation: 153.84
Mean False Positive: 346.1, Standard Deviation: 153.84
Mean False Negative: 561.8, Standard Deviation: 181.54

Precision: 0.842
Recall: 0.766
False Positive Rate: 0.144
F-Measure: 0.802
```

As you can see 90% of the documents from the chosen range are used as training set and 10% for testing, that is not part of Mitchell's routine. Also, ten-fold cross validation is implemented and the mean values are the result of the ten different learning and testing combinations as well as the Precision and Recall rates and F-Measure.

Look at previous commits for simpler code (string comparison instead of using hash, for example).
