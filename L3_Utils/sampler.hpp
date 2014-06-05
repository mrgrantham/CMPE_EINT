/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

#ifndef SAMPLER_HPP_
#define SAMPLER_HPP_

/**
 * Sampler class.
 * The purpose of this class is to store samples of a variable type
 * and be able to get the average, low, high from the samples.
 *
 * @code
 * Sampler<int> samples(2);
 * samples.storeSample(10);
 * samples.storeSample(20);
 * int avg = samples.getAverage(); // Should be 15
 * @endcode
 */
template <typename TYPE>
class Sampler
{
    public:
        /**
         * Constructor of the class
         * @param [in] numSamples   The maximum number of samples after which samples are stored as circular buffer
         */
        Sampler(int numSamples) : mSampleArraySize(numSamples), mSampleIndex(0), mAllSamplesReady(false)
        {
            mSamples = new TYPE[numSamples];
            for(int i=0; i < numSamples; i++) {
                mSamples[i] = 0;
            }
        }

        /// Destructor of the class
        ~Sampler()
        {
            delete [] mSamples;
        }

        /**
         * Store a sample into the circular buffer
         * @param [in] sample   The sample value
         */
        void storeSample(const TYPE& sample)
        {
            mSamples[mSampleIndex] = sample;
            if(++mSampleIndex >= mSampleArraySize) {
                mSampleIndex = 0;
                mAllSamplesReady = true;
            }
        }

        /// @returns The average of all the samples
        TYPE getAverage(void) const
        {
            const int numSamples = getSampleCount();
            TYPE sum = 0;

            for(int i=0; i < numSamples; i++) {
                sum += mSamples[i];
            }

            return (sum / numSamples);
        }

        /// @returns The latest sample that was stored using storeSample()
        TYPE getLatest(void) const
        {
            int idx = (0 == mSampleIndex) ? (mSampleArraySize - 1) : (mSampleIndex - 1);
            return mSamples[idx];
        }

        /// @returns The highest sample available from the circular buffer
        TYPE getHighest(void) const
        {
            const int numSamples = getSampleCount();
            TYPE highest = mSamples[0];

            for(int i=0; i < numSamples; i++) {
                if (highest < mSamples[i]) {
                    highest = mSamples[i];
                }
            }

            return highest;
        }

        /// @returns The lowest sample available from the circular buffer
        TYPE getLowest(void) const
        {
            const int numSamples = getSampleCount();
            TYPE lowest = mSamples[0];

            for(int i=0; i < numSamples; i++) {
                if (lowest > mSamples[i]) {
                    lowest = mSamples[i];
                }
            }

            return lowest;
        }

        /// @returns true if all of the circular buffer has all the samples stored
        inline bool allSamplesReady(void)  const { return mAllSamplesReady; }

        /// @returns the maximum number of samples we can store
        inline int getMaxSampleCount(void) const { return mSampleArraySize; }

        /// @returns the number of samples we have actually stored using storeSample()
        inline int getSampleCount(void)    const { return mAllSamplesReady ? mSampleArraySize : mSampleIndex; }

        /// @returns the sample located at the given index
        inline TYPE getSampleNum(int idx)  const { return mSamples[idx % mSampleArraySize]; }

        /// Clears all the samples at this class, and resets allSamplesReady() flag
        void clear(void)
        {
            mAllSamplesReady = false;
            mSampleIndex = 0;
        }

    private:
        /// Do not use this constructor
        Sampler() :
            mSampleArraySize(0), mSampleIndex(0),
            mAllSamplesReady(false), mSamples(0)
        {
        }

        const int mSampleArraySize; ///< Number of samples
        int mSampleIndex;           ///< Index of next sample that will get stored to mSamples array
        bool mAllSamplesReady;      ///< If the whole array is not filled, we can't compute the average
        TYPE* mSamples;             ///< Array of samples
};

#endif /* SAMPLER_HPP_ */
