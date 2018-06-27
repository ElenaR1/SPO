package threads.calculateE;

import java.math.BigDecimal;

public class CalculateE {

    public static int precision;

    public static BigDecimal calculateSumMember(int x) {
        BigDecimal numerator = new BigDecimal(3 - 4 * x * x);
        numerator = numerator.setScale(precision);

        BigDecimal denominator = new BigDecimal(factorial(2 * x + 1));
        denominator = denominator.setScale(precision);

        return numerator.divide(denominator, BigDecimal.ROUND_HALF_UP);
    }

    public static String factorial(int n) {
        BigDecimal factorial = new BigDecimal(1);
        factorial = factorial.setScale(precision);
        if (n == 0 || n == 1) {
            return factorial.toString();
        }
        while (n > 1) {
            BigDecimal N = new BigDecimal(n);
            N = N.setScale(precision);
            factorial = factorial.multiply(N);
            n--;
        }
        return factorial.toString();
    }
}