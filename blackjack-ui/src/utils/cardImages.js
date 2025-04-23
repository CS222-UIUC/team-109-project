export function cardImageUrl(card) {
    // e.g. { rank: 'K', suit: 'Hearts' } → 'KH'
    const suitMap = { Hearts: 'hearts', Diamonds: 'diamonds', Clubs: 'clubs', Spades: 'spades' };
    console.log(`/cards/${card.rank}_of_${suitMap[card.suit]}.png`)
    return `/cards/${card.rank}_of_${suitMap[card.suit]}.png`;
}