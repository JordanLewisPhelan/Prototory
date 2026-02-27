
### Murmur Hash Algorithm Influences
First instance of reviewing pros and cons - mostly covers the base explanation and isn't really appliable in described context - BUT it highlighted the idea and furthered my research into this area.
- [Medium Website Blog on MurmurHash & Cryptology](https://medium.com/@thealonemusk/murmurhash-the-scrappy-algorithm-that-secretly-powers-half-the-internet-2d3f79b4509b)

### std::Chrono Influences & experimenting
Went on a little bit of a tangented search trying to seed efficiently - I am honestly a little not tired but upset with SRand and rand(), they are good but they are clunky, due to how they function they need to be randomized several times to prevent pattern instancing. I was looking through SFML documentation for SF::Time and stumbled upon std::chrono as it was listed as an extra method to use alongside sf::Time, so I am going to try and dabble with that and see how it performs. 
- [Chrono::Duration page - Tangent where I explored some inner elements](https://en.cppreference.com/w/cpp/chrono/duration.html)

- [SFML Page that began the rabbit hole.](https://www.sfml-dev.org/documentation/3.0.0/classsf_1_1Time.html)